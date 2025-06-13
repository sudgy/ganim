#include "section_render.hpp"

#include <limits>

using namespace ganim;
using namespace ganim::gex;

Box gex::section_render(const Section& section)
{
    if (section.type == Section::Text) {
        auto& font = get_font("fonts/NewCM10-Regular.otf");
        auto codepoints = std::vector<std::pair<std::u32string, int>>();
        for (auto& token : section.tokens) {
            if (codepoints.empty()) {
                codepoints.emplace_back(U"", token.group);
            }
            else {
                if (codepoints.back().second != token.group) {
                    codepoints.emplace_back(U"", token.group);
                }
            }
            codepoints.back().first.push_back(
                std::get<CharacterToken>(token.value).codepoint
            );
        }
        auto glyphs = shape_text_manual_groups(font, codepoints);
        if (glyphs.empty()) {
            return {};
        }
        auto left_x_pos = std::numeric_limits<double>::infinity();
        auto right_x_pos = -std::numeric_limits<double>::infinity();
        auto y_pos = glyphs.front().y_pos;
        auto top_y_pos = -std::numeric_limits<double>::infinity();
        auto bottom_y_pos = std::numeric_limits<double>::infinity();
        for (auto& glyph : glyphs) {
            left_x_pos = std::min(left_x_pos, glyph.x_pos);
            right_x_pos = std::max(right_x_pos, glyph.x_pos + glyph.width);
            top_y_pos = std::max(top_y_pos, glyph.draw_y);
            bottom_y_pos = std::min(bottom_y_pos, glyph.draw_y - glyph.height);
        }
        return Box(
                right_x_pos - left_x_pos,
                top_y_pos - y_pos,
                y_pos - bottom_y_pos,
                glyphs
            );
    }
    else {
        return {};
    }
}
