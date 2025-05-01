#include "text.hpp"

#include "text_helpers.hpp"

using namespace ganim;

Text::Text(const std::vector<std::string_view>& strings) : Text({}, strings) {}

Text::Text(TextArgs args, const std::vector<std::string_view>& strings)
{
    draw_together();
    set_draw_subobject_ratio(0.2);
    M_font = &get_font(std::string(args.font_filename), args.font_pixel_size);
    M_newline_buff = args.newline_buff;
    create(strings);
}

std::vector<PositionedGlyph> Text::get_glyphs(
        const std::vector<std::u32string>& strings)
{
    auto shaped_glyphs_per_line = std::vector<std::vector<ShapedGlyph>>();
    auto later_strings = strings;
    later_strings.push_back(U"\n");
    for (int i = 0; i < ssize(later_strings); ++i) {
        while (true) {
            auto pos = later_strings[i].find_first_of('\n');
            if (pos == std::string_view::npos) break;
            auto earlier_strings = later_strings;
            earlier_strings.resize(i+1);
            earlier_strings[i] = earlier_strings[i].substr(0, pos);
            shaped_glyphs_per_line.push_back(
                    shape_text(*M_font, earlier_strings));
            for (int j = 0; j < i; ++j) {
                later_strings[j] = U"";
            }
            later_strings[i] = later_strings[i].substr(pos+1);
        }
    }

    auto result = std::vector<PositionedGlyph>();
    auto y_plus = 0.0;
    const auto ascender = get_font_ascender(*M_font);
    const auto descender = get_font_descender(*M_font);

    for (auto& shaped_glyphs : shaped_glyphs_per_line) {
        auto line_start = ssize(result);
        auto x_min = double(INFINITY);
        auto x_max = double(-INFINITY);
        for (auto& glyph : shaped_glyphs) {
            auto& c = *glyph.glyph;
            x_min = std::min(x_min, glyph.x_pos + c.bearing_x);
            x_max = std::max(x_max, glyph.x_pos + c.bearing_x);
            x_min = std::min(x_min, glyph.x_pos + c.bearing_x + c.width);
            x_max = std::max(x_max, glyph.x_pos + c.bearing_x + c.width);
            result.emplace_back(
                glyph.x_pos + c.bearing_x,
                c.bearing_y + glyph.y_pos + y_plus,
                c.width,
                -c.height,
                glyph.y_pos + descender + y_plus,
                glyph.y_pos + ascender + y_plus,
                c.texture_x,
                c.texture_y,
                c.texture_width,
                c.texture_height,
                glyph.group_index
            );
        }
        const auto x_shift = -(x_min + x_max) / 2;
        for (int i = line_start; i < ssize(result); ++i) {
            result[i].x += x_shift;
        }
        y_plus -= ascender - descender + M_newline_buff;
    }
    return result;
}
