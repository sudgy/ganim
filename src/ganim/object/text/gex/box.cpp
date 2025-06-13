#include "box.hpp"

using namespace ganim;
using namespace ganim::gex;

Box gex::combine_boxes_horizontally(const std::vector<Box>& boxes)
{
    auto result = Box();
    for (auto& box : boxes) {
        for (const auto& glyph : box.glyphs) {
            auto& new_glyph = result.glyphs.emplace_back(glyph);
            new_glyph.x_pos += result.width;
            new_glyph.draw_x += result.width;
        }
        result.width += box.width;
        result.height = std::max(result.height, box.height);
        result.depth = std::max(result.depth, box.depth);
    }
    return result;
}

Box gex::combine_boxes_vertically(
    const std::vector<Box>& boxes,
    int reference_box
)
{
    if (boxes.size() == 0) return {};
    auto result = Box();
    auto pluses_zero_reference = std::vector<double>();
    pluses_zero_reference.push_back(-boxes.front().height);
    for (int i = 0; i < ssize(boxes); ++i) {
        auto& box = boxes[i];
        pluses_zero_reference.back() += box.height;
        pluses_zero_reference.push_back(box.depth);
        result.width = std::max(result.width, box.width);
        if (i <= reference_box) {
            result.height += box.height;
        }
        if (i < reference_box) {
            result.height += box.depth;
        }
        if (i >= reference_box) {
            result.depth += box.depth;
        }
        if (i > reference_box) {
            result.depth += box.height;
        }
    }
    for (int i = 0; i < ssize(boxes); ++i) {
        auto y_plus = pluses_zero_reference[reference_box]
            - pluses_zero_reference[i];
        for (const auto& glyph : boxes[i].glyphs) {
            auto& new_glyph = result.glyphs.emplace_back(glyph);
            new_glyph.y_pos += y_plus;
            new_glyph.draw_y += y_plus;
            new_glyph.y_min += y_plus;
            new_glyph.y_max += y_plus;
        }
    }
    return result;
}
