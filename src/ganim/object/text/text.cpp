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
    auto shaped_glyphs_per_line = std::vector<std::vector<PositionedGlyph>>();
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
            auto& new_glyph = result.emplace_back(glyph);
            x_min = std::min(x_min, glyph.draw_x);
            x_max = std::max(x_max, glyph.draw_x);
            x_min = std::min(x_min, glyph.draw_x + glyph.width);
            x_max = std::max(x_max, glyph.draw_x + glyph.width);
            new_glyph.y_pos += y_plus;
            new_glyph.draw_y += y_plus;
            new_glyph.y_min += y_plus;
            new_glyph.y_max += y_plus;
        }
        const auto x_shift = -(x_min + x_max) / 2;
        for (int i = line_start; i < ssize(result); ++i) {
            result[i].x_pos += x_shift;
            result[i].draw_x += x_shift;
        }
        y_plus -= ascender - descender + M_newline_buff;
    }
    return result;
}

ObjectPtr<Text> Text::polymorphic_copy() const
{
    return ObjectPtr<Text>::from_new(polymorphic_copy_impl());
}

Text* Text::polymorphic_copy_impl() const
{
    return new Text(*this);
}
