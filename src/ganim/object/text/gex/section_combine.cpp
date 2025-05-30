#include "section_combine.hpp"

#include "ganim/fmap.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<PositionedGlyph> gex::section_combine(
    const std::vector<RenderedSection>& sections
)
{
    auto result = std::vector<PositionedGlyph>();
    result.reserve(sections.size() *
            (sections.empty() ? 0 : sections[0].glyphs.size()));
    auto [_, pluses] = combine_boxes_horizontally(
        fmap(sections, [](const auto& s) {return s.box;})
    );
    for (int i = 0; i < ssize(sections); ++i) {
        auto s = ssize(result);
        result.append_range(sections[i].glyphs);
        for (int j = s; j < ssize(result); ++j) {
            result[j].x_pos += pluses[i].first;
            result[j].y_pos += pluses[i].second;
            result[j].draw_x += pluses[i].first;
            result[j].draw_y += pluses[i].second;
            result[j].y_min += pluses[i].second;
            result[j].y_max += pluses[i].second;
        }
    }

    return result;
}
