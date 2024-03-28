#include "character.hpp"

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_ADVANCES_H
#include "ganim/gl/gl.hpp"

#include "ganim/gl/texture.hpp"

using namespace ganim;

namespace {
    std::unordered_map<std::string, Font> G_fonts;
    FT_Library G_freetype;
    gl::Texture G_text_texture = 0;
    int G_tt_x = 0;
    int G_tt_y = 0;
    int G_tt_h = 0;
    constexpr auto GC_pixel_size = 128.0;
}

struct ganim::Font {
    inline static int S_count = 0;
    FT_Face M_face;
    std::unordered_map<int, Character> M_characters;
    Font(const std::string& filename)
    {
        if (S_count == 0) {
            G_text_texture = gl::Texture();
            glBindTexture(GL_TEXTURE_2D, G_text_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                GC_default_text_texture_size, GC_default_text_texture_size,
                0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            auto error = FT_Init_FreeType(&G_freetype);
            if (error) {
                std::cerr << "Error initializing FreeType\n";
            }
        }
        ++S_count;
        auto error = FT_New_Face(G_freetype, filename.c_str(), 0, &M_face);
        if (error) {
            std::cerr << "Unable to open font " << filename << "\n";
        }
        // TODO: Don't be this terrible with extensions
        auto filename2 = filename;
        filename2[filename2.size() - 1] = 'm';
        error = FT_Attach_File(M_face, filename2.c_str());
        if (error) {
            std::cerr << "Unable to read kerning metrics for font " << filename
                      << "\n";
        }
        // TODO: Make this configurable
        error = FT_Set_Pixel_Sizes(M_face, 0, GC_pixel_size);
        if (error) {
            std::cerr << "Unable to set font size " << filename << "\n";
        }
    }
    Font(const Font&)=delete;
    Font& operator=(const Font&)=delete;
    Font(Font&& other)
    {
        ++S_count;
        M_face = other.M_face;
        other.M_face = nullptr;
    }
    Font& operator=(Font&& other)
    {
        if (this != &other) {
            M_face = other.M_face;
            other.M_face = nullptr;
        }
        return *this;
    }
    ~Font()
    {
        if (M_face != nullptr) FT_Done_Face(M_face);
        --S_count;
        if (S_count == 0) {
            G_text_texture = 0;
            auto error = FT_Done_FreeType(G_freetype);
            if (error) {
                std::cerr << "Error cleaning up FreeType\n";
            }
        }
    }
};

Font& ganim::get_font(const std::string& filename)
{
    return G_fonts.emplace(filename, Font(filename)).first->second;
}

Character& ganim::get_character(Font& font, UnicodeCodepoint character)
{
    auto face = font.M_face;
    auto codepoint = character.codepoint;
    auto& result = font.M_characters[codepoint];
    if (result.width != 0) return result;
    auto glyph_index = FT_Get_Char_Index(face, codepoint);
    auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        std::cerr << "Error loading glyph with unicode codepoint "
                  << codepoint << "\n";
    }
    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    }

    auto& bitmap = face->glyph->bitmap;
    auto width = int(bitmap.width);
    auto height = int(bitmap.rows);
    auto rgba_buffer = std::make_unique<unsigned char[]>(width * height * 4);
    for (int i = 0; i < width * height; ++i) {
        rgba_buffer[i*4 + 0] = bitmap.buffer[i];
        rgba_buffer[i*4 + 1] = bitmap.buffer[i];
        rgba_buffer[i*4 + 2] = bitmap.buffer[i];
        rgba_buffer[i*4 + 3] = 255;
    }
    if (G_tt_x + width + 2 > GC_default_text_texture_size) {
        G_tt_x = 0;
        G_tt_y += G_tt_h;
        G_tt_h = 0;
    }
    glBindTexture(GL_TEXTURE_2D, G_text_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
            G_tt_x + 1, G_tt_y + 1, width, height,
            GL_RGBA, GL_UNSIGNED_BYTE, rgba_buffer.get());

    result.texture_x = static_cast<double>(G_tt_x) / GC_default_text_texture_size;
    result.texture_y = static_cast<double>(G_tt_y) / GC_default_text_texture_size;
    result.texture_width = static_cast<double>(width + 2) / GC_default_text_texture_size;
    result.texture_height = static_cast<double>(height + 2) / GC_default_text_texture_size;
    result.width = (bitmap.width + 2) / GC_pixel_size;
    result.height = (bitmap.rows + 2) / GC_pixel_size;
    result.bearing_x = (face->glyph->bitmap_left - 1) / GC_pixel_size;
    result.bearing_y = (face->glyph->bitmap_top + 1) / GC_pixel_size;
    result.x_advance = face->glyph->advance.x / 64.0 / GC_pixel_size;
    auto advance = FT_Fixed();
    FT_Get_Advance(face, glyph_index, FT_LOAD_NO_SCALE, &advance);
    result.x_advance_em = static_cast<double>(advance) / face->units_per_EM;

    G_tt_x += width + 2;
    G_tt_y += height + 2;
    G_tt_h = std::max(G_tt_h, height + 2);
    return result;
}

double ganim::get_font_pem(Font& font)
{
    return font.M_face->size->metrics.x_ppem / GC_pixel_size;
}

double ganim::get_kerning(
        Font& font, UnicodeCodepoint left, UnicodeCodepoint right)
{
    auto face = font.M_face;
    auto left_glyph_index = FT_Get_Char_Index(face, left.codepoint);
    auto right_glyph_index = FT_Get_Char_Index(face, right.codepoint);
    auto kerning = FT_Vector();
    auto error = FT_Get_Kerning(font.M_face,
            left_glyph_index, right_glyph_index, FT_KERNING_DEFAULT, &kerning);
    if (error) {
        std::cerr << "Error in find kerning between characters "
                  << left.codepoint << " and " << right.codepoint << "\n";
    }
    return static_cast<double>(kerning.x) / 64 / GC_pixel_size;
}

unsigned ganim::get_text_texture()
{
    return G_text_texture;
}
