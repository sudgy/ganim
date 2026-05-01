#include "gex.hpp"

#include "gex/gex.hpp"
#include "ganim/fmap.hpp"

using namespace ganim;

namespace {
    std::vector<std::string> split_tex_strings(
        const std::vector<std::string_view>& tex_strings
    )
    {
        auto result = std::vector<std::string>();
        result.reserve(tex_strings.size());
        auto pos = std::string::npos;
        for (std::string_view str : tex_strings) {
            while ((pos = str.find("  ")) != std::string::npos) {
                if (pos != 0) {
                    result.emplace_back(str.substr(0, pos));
                }
                str.remove_prefix(pos + 2);
            }
            if (str.size() > 0) result.emplace_back(str);
        }
        return result;
    }
}

Gex::Gex(bool math, int pixel_size,const std::vector<std::string_view>& strings)
:   M_math(math),
    M_pixel_size(pixel_size)
{
    M_tex_strings = split_tex_strings(strings);
    draw_together();
    set_draw_subobject_ratio(0.2);
    M_font = &get_font("fonts/NewCM10-Regular.otf", pixel_size);
    auto string_views = std::vector<std::string_view>();
    string_views.reserve(M_tex_strings.size());
    for (auto& str : M_tex_strings) string_views.push_back(str);
    create(string_views);

    set_colors(default_color_map);
    scale(default_scale);
}

std::vector<Glyph> Gex::get_glyphs(
        const std::vector<std::string_view>& strings)
{
    auto result = gex_render(M_math, M_pixel_size, strings);
    auto x_min = double(INFINITY);
    auto x_max = -double(INFINITY);
    for (auto& glyph : result.glyphs) {
        x_min = std::min(x_min, glyph.draw_x);
        x_min = std::min(x_min, glyph.draw_x + glyph.width);
        x_max = std::max(x_max, glyph.draw_x);
        x_max = std::max(x_max, glyph.draw_x + glyph.width);
    }
    const auto x_shift = -(x_min + x_max) / 2;
    for (auto& glyph : result.glyphs) {
        glyph.x_pos += x_shift;
        glyph.draw_x += x_shift;
    }
    return result.glyphs;
}

ObjectPtr<Gex> Gex::copy() const
{
    return ObjectPtr<Gex>::from_new(copy_impl());
}

Gex* Gex::copy_impl() const
{
    return new Gex(*this);
}

void Gex::set_colors(const std::unordered_map<std::string, Color>& colors)
{
    for (auto i = 0; i < ssize(M_tex_strings); ++i) {
        for (auto& [str, color] : colors) {
            auto& tex = M_tex_strings[i];
            if (tex.find(str) != tex.npos) {
                (*this)[i]->set_color(color);
                break;
            }
        }
    }
}

double Gex::get_axis_y() const
{
    return get_origin().undual().blade_project<pga3::e2>()
        + get_font_axis_height(*M_font);
}
