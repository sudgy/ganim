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

Gex::Gex(const std::vector<std::string_view>& strings)
{
    M_tex_strings = split_tex_strings(strings);
    draw_together();
    set_draw_subobject_ratio(0.2);
    M_font = &get_font("fonts/NewCM10-Regular.otf", 128);
    auto string_views = std::vector<std::string_view>();
    string_views.reserve(M_tex_strings.size());
    for (auto& str : M_tex_strings) string_views.push_back(str);
    create(string_views);
}

std::vector<PositionedGlyph> Gex::get_glyphs(
        const std::vector<std::string_view>& strings)
{
    return gex_render(strings);
}

ObjectPtr<Gex> Gex::polymorphic_copy() const
{
    return ObjectPtr<Gex>::from_new(polymorphic_copy_impl());
}

Gex* Gex::polymorphic_copy_impl() const
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
