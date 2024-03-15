#include "character.hpp"

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
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
    constexpr auto GC_ganim_unit = 120.0; // TODO: Don't hardcode this!
}

UnicodeCodepoint::UnicodeCodepoint(int codepoint) : codepoint(codepoint) {}

UnicodeCodepoint::UnicodeCodepoint(std::string_view utf8)
{
    auto string = reinterpret_cast<const std::uint8_t*>(utf8.data());
    if (string[0] & 0x80) {
        if (string[0] >> 5 == 0b110) {
            codepoint = static_cast<int>(string[1] & 0b00111111) +
                       (static_cast<int>(string[0] & 0b00011111) << 6);
        }
        else if (string[0] >> 4 == 0b1110) {
            codepoint = static_cast<int>(string[2] & 0b00111111) +
                       (static_cast<int>(string[1] & 0b00111111) << 6) +
                       (static_cast<int>(string[0] & 0b00001111) << 12);
        }
        // We're evil and don't check for valid UTF-8 >:)
        else {
            codepoint = static_cast<int>(string[3] & 0b00111111) +
                       (static_cast<int>(string[2] & 0b00111111) << 6) +
                       (static_cast<int>(string[1] & 0b00111111) << 12) +
                       (static_cast<int>(string[0] & 0b00000111) << 18);
        }
    }
    else codepoint = string[0];
}

UnicodeCodepoint::UnicodeCodepoint(char ascii) : codepoint(ascii) {}

UnicodeCodepoint UnicodeCodepoint::from_utf8(std::string_view utf8)
{
    return UnicodeCodepoint(utf8);
}

UnicodeCodepoint UnicodeCodepoint::from_utf16(std::string_view utf16)
{
    auto string = reinterpret_cast<const std::uint16_t*>(utf16.data());
    if (string[0] < 0xD7FF or string[0] >= 0xE000) return string[0];
    auto high = static_cast<std::uint32_t>(string[0] - 0xD800);
    auto low = static_cast<std::uint32_t>(string[1] - 0xDC00);
    return static_cast<int>((high << 10) + low + 0x10000U);
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
        error = FT_Set_Pixel_Sizes(M_face, 0, 64);
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
    result.width = (bitmap.width + 2) / GC_ganim_unit;
    result.height = (bitmap.rows + 2) / GC_ganim_unit;
    result.bearing_x = (face->glyph->bitmap_left - 1) / GC_ganim_unit;
    result.bearing_y = (face->glyph->bitmap_top + 1) / GC_ganim_unit;
    result.x_advance = (face->glyph->advance.x + 2) / 64 / GC_ganim_unit;

    G_tt_x += width + 2;
    G_tt_y += height + 2;
    G_tt_h = std::max(G_tt_h, height + 2);
    return result;
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
    return static_cast<double>(kerning.x) / 64 / GC_ganim_unit;
}

unsigned ganim::get_text_texture()
{
    return G_text_texture;
}
