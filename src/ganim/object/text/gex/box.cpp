#include "box.hpp"

#include <limits>

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

Box gex::box_from_glyphs(std::vector<PositionedGlyph> glyphs)
{
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

void gex::scale_box(Box& box, double scale)
{
    box.width *= scale;
    box.height *= scale;
    box.depth *= scale;
    for (auto& glyph : box.glyphs) {
        glyph.x_pos *= scale;
        glyph.y_pos *= scale;
        glyph.draw_x *= scale;
        glyph.draw_y *= scale;
        glyph.width *= scale;
        glyph.height *= scale;
        glyph.y_min *= scale;
        glyph.y_max *= scale;
    }
}

void gex::vertical_shift_box(Box& box, double amount)
{
    box.height += amount;
    box.depth -= amount;
    for (auto& glyph : box.glyphs) {
        glyph.y_pos += amount;
        glyph.draw_y += amount;
        glyph.y_min += amount;
        glyph.y_max += amount;
    }
}

void gex::horizontal_shift_box(Box& box, double amount)
{
    box.width += amount;
    for (auto& glyph : box.glyphs) {
        glyph.x_pos += amount;
        glyph.draw_x += amount;
    }
}
