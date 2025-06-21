#include "text_helpers.hpp"

#include <unordered_map>
#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_ADVANCES_H
#include <hb.h>
#include <hb-ft.h>
#include "ganim/gl/gl.hpp"

#include "ganim/gl/texture.hpp"

using namespace ganim;

namespace {
    struct pair_hash {
        std::size_t operator()(const std::pair<std::string, int>& pair) const
        {
            auto h1 = std::hash<std::string>{}(pair.first);
            auto h2 = std::hash<int>{}(pair.second);
            return h1 ^ h2;
        }
    };
    std::unordered_map<std::pair<std::string, int>, Font, pair_hash> G_fonts;
    FT_Library G_freetype;
    gl::Texture G_text_texture = 0;
    int G_tt_x = 2; // A single white pixel is placed on the corner for rules
    int G_tt_y = 0;
    int G_tt_h = 2;
}

struct ganim::Font {
    inline static int S_count = 0;
    FT_Face M_ft_face;
    hb_font_t* M_hb_font = nullptr;
    std::unordered_map<glyph_t, Glyph> M_glyphs;
    double M_pixel_size = 0;
    Font(const std::string& filename, int pixel_size)
        : M_pixel_size(pixel_size)
    {
        if (S_count == 0) {
            G_text_texture = gl::Texture();
            glBindTexture(GL_TEXTURE_2D, G_text_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                GC_default_text_texture_size, GC_default_text_texture_size,
                0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            const std::uint8_t rules[] = {0xFF, 0xFF, 0xFF, 0xFF};
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, 1, 1,
                    GL_RGBA, GL_UNSIGNED_BYTE, rules);
            auto error = FT_Init_FreeType(&G_freetype);
            if (error) {
                throw std::runtime_error("Error initializing FreeType");
            }
        }
        ++S_count;
        auto error = FT_New_Face(G_freetype, filename.c_str(), 0, &M_ft_face);
        if (error) {
            throw std::runtime_error(
                    std::format("Unable to open font {}", filename));
        }
        error = FT_Set_Pixel_Sizes(M_ft_face, 0, pixel_size);
        if (error) {
            std::cerr << "Unable to set font size " << filename << "\n";
        }
        M_hb_font = hb_ft_font_create_referenced(M_ft_face);
        hb_ft_font_set_funcs(M_hb_font);
    }
    Font(const Font&)=delete;
    Font& operator=(const Font&)=delete;
    Font(Font&& other) : M_pixel_size(other.M_pixel_size)
    {
        ++S_count;
        M_ft_face = other.M_ft_face;
        other.M_ft_face = nullptr;
        M_hb_font = other.M_hb_font;
        other.M_hb_font = nullptr;
    }
    Font& operator=(Font&& other)
    {
        if (this != &other) {
            M_pixel_size = other.M_pixel_size;
            if (M_hb_font) hb_font_destroy(M_hb_font);
            M_ft_face = other.M_ft_face;
            other.M_ft_face = nullptr;
            M_hb_font = other.M_hb_font;
            other.M_hb_font = nullptr;
        }
        return *this;
    }
    ~Font()
    {
        if (M_hb_font) hb_font_destroy(M_hb_font);
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

Font& ganim::get_font(const std::string& filename, int pixel_size)
{
    return G_fonts.emplace(std::make_pair(filename, pixel_size), Font(filename, pixel_size)).first->second;
}

Glyph& ganim::get_glyph(Font& font, glyph_t glyph_index)
{
    auto face = font.M_ft_face;
    auto& result = font.M_glyphs[glyph_index];
    if (result.width != 0) return result;
    auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        throw std::runtime_error(std::format(
                    "Error loading glyph with glyph index {}", glyph_index));
    }
    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    }

    auto& bitmap = face->glyph->bitmap;
    auto width = int(bitmap.width);
    auto height = int(bitmap.rows);
    auto rgba_buffer = std::make_unique<unsigned char[]>(width * height * 4);
    for (int i = 0; i < width * height; ++i) {
        rgba_buffer[i*4 + 0] = 255;
        rgba_buffer[i*4 + 1] = 255;
        rgba_buffer[i*4 + 2] = 255;
        rgba_buffer[i*4 + 3] = bitmap.buffer[i];
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
    result.width = (bitmap.width + 2) / font.M_pixel_size;
    result.height = (bitmap.rows + 2) / font.M_pixel_size;
    result.bearing_x = (face->glyph->bitmap_left - 1) / font.M_pixel_size;
    result.bearing_y = (face->glyph->bitmap_top + 1) / font.M_pixel_size;

    G_tt_x += width + 2;
    G_tt_h = std::max(G_tt_h, height + 2);
    return result;
}

double ganim::get_font_ascender(Font& font)
{
    auto face = font.M_ft_face;
    auto glyph_index = FT_Get_Char_Index(face, '|');
    FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    }
    return face->glyph->bitmap_top / font.M_pixel_size;
}

double ganim::get_font_descender(Font& font)
{
    auto face = font.M_ft_face;
    auto glyph_index = FT_Get_Char_Index(face, '|');
    FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    }
    return (face->glyph->bitmap_top - int(face->glyph->bitmap.rows))
        / font.M_pixel_size;
}

std::vector<PositionedGlyph> ganim::shape_text(
    Font& font,
    const std::vector<std::u32string>& text
)
{
    auto group_index = 0;
    auto new_text = std::vector<std::pair<std::u32string, int>>();
    new_text.reserve(text.size());
    for (auto& string : text) {
        new_text.emplace_back(string, group_index++);
    }
    return shape_text_manual_groups(font, new_text);
}

std::vector<PositionedGlyph> ganim::shape_text_manual_groups(
    Font& font,
    const std::vector<std::pair<std::u32string, int>>& text
)
{
    auto buffer = hb_buffer_create();
    for (auto& [string, group_index] : text) {
        for (auto codepoint : string) {
            hb_buffer_add(buffer, codepoint, group_index);
        }
    }
    hb_buffer_set_content_type(buffer, HB_BUFFER_CONTENT_TYPE_UNICODE);
    hb_buffer_guess_segment_properties(buffer);
    hb_shape(font.M_hb_font, buffer, nullptr, 0);

    auto glyph_count = 0U;
    auto glyph_infos = hb_buffer_get_glyph_infos(buffer, &glyph_count);
    auto glyph_positions = hb_buffer_get_glyph_positions(buffer, &glyph_count);

    auto result = std::vector<PositionedGlyph>();
    result.resize(glyph_count);

    auto cursor_x = 0;
    auto cursor_y = 0;
    const auto ascender = get_font_ascender(font);
    const auto descender = get_font_descender(font);
    for (auto i = 0U; i < glyph_count; ++i) {
        auto& glyph = result[i];
        auto& glyph_data = get_glyph(font, glyph_infos[i].codepoint);
        glyph.x_pos = (cursor_x + glyph_positions[i].x_offset)
            / 64.0 / font.M_pixel_size;
        glyph.y_pos = (cursor_y + glyph_positions[i].y_offset)
            / 64.0 / font.M_pixel_size;
        glyph.draw_x = glyph.x_pos + glyph_data.bearing_x;
        glyph.draw_y = glyph.y_pos + glyph_data.bearing_y;
        glyph.width = glyph_data.width;
        glyph.height = glyph_data.height;
        glyph.y_min = glyph.y_pos + descender;
        glyph.y_max = glyph.y_pos + ascender;
        glyph.texture_x = glyph_data.texture_x;
        glyph.texture_y = glyph_data.texture_y;
        glyph.texture_width = glyph_data.texture_width;
        glyph.texture_height = glyph_data.texture_height;
        glyph.group_index = glyph_infos[i].cluster;
        cursor_x += glyph_positions[i].x_advance;
        cursor_y += glyph_positions[i].y_advance;
    }
    hb_buffer_destroy(buffer);
    return result;
}

unsigned ganim::get_text_texture()
{
    return G_text_texture;
}

void ganim::set_text_texture_size(int size)
{
    G_text_texture = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, G_text_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    const std::uint8_t rules[] = {0xFF, 0xFF, 0xFF, 0xFF};
    glTexSubImage2D(GL_TEXTURE_2D, 0,
            0, 0, 1, 1,
            GL_RGBA, GL_UNSIGNED_BYTE, rules);
    G_tt_x = 2;
    G_tt_y = 0;
    G_tt_h = 2;
    for (auto& [_, font] : G_fonts) {
        font.M_glyphs.clear();
    }
}

double ganim::get_font_em(Font& font)
{
    return font.M_ft_face->size->metrics.x_ppem / font.M_pixel_size;
}

double ganim::get_font_x_height(Font& font)
{
    return get_font_em(font)*4.0/9.0;
}

double ganim::get_font_quad(Font& font)
{
    return get_font_em(font)*1.15;
}

double ganim::get_font_num1(Font& font)
{
    return get_font_em(font)*0.746712;
}

double ganim::get_font_num2(Font& font)
{
    return get_font_em(font)*0.423933;
}

double ganim::get_font_num3(Font& font)
{
    return get_font_em(font)*0.473933;
}

double ganim::get_font_denom1(Font& font)
{
    return get_font_em(font)*0.756155;
}

double ganim::get_font_denom2(Font& font)
{
    return get_font_em(font)*0.375043;
}

double ganim::get_font_sup1(Font& font)
{
    return get_font_em(font)*0.412892;
}

double ganim::get_font_sup2(Font& font)
{
    return get_font_em(font)*0.362892;
}

double ganim::get_font_sup3(Font& font)
{
    return get_font_em(font)*0.288889;
}

double ganim::get_font_sub1(Font& font)
{
    return get_font_em(font)*0.15;
}

double ganim::get_font_sub2(Font& font)
{
    return get_font_em(font)*0.309219;
}

double ganim::get_font_sup_drop(Font& font)
{
    return get_font_em(font)*0.386108;
}

double ganim::get_font_sub_drop(Font& font)
{
    return get_font_em(font)*0.05;
}

double ganim::get_font_delim1(Font& font)
{
    return get_font_em(font)*2.39;
}

double ganim::get_font_delim2(Font& font)
{
    return get_font_em(font)*1.01;
}

double ganim::get_font_axis_height(Font& font)
{
    return get_font_em(font)*0.25;
}

double ganim::get_font_default_rule_thickness(Font& font)
{
    return get_font_em(font)*0.04;
}

double ganim::get_font_big_op_spacing1(Font& font)
{
    return get_font_em(font)/9.0;
}

double ganim::get_font_big_op_spacing2(Font& font)
{
    return get_font_em(font)/6.0;
}

double ganim::get_font_big_op_spacing3(Font& font)
{
    return get_font_em(font)*0.2;
}

double ganim::get_font_big_op_spacing4(Font& font)
{
    return get_font_em(font)*0.6;
}

double ganim::get_font_big_op_spacing5(Font& font)
{
    return get_font_em(font)*0.1;
}
