#include "box.hpp"

using namespace ganim;
using namespace ganim::gex;

BoxCombineResult gex::combine_boxes_horizontally(const std::vector<Box>& boxes)
{
    auto result = BoxCombineResult();
    for (auto& box : boxes) {
        auto& pluses = result.pluses.emplace_back();
        pluses.first = result.new_box.width;
        result.new_box.width += box.width;
        result.new_box.height = std::max(result.new_box.height, box.height);
        result.new_box.depth = std::max(result.new_box.depth, box.depth);
    }
    return result;
}

BoxCombineResult gex::combine_boxes_vertically(
    const std::vector<Box>& boxes,
    int reference_box
)
{
    if (boxes.size() == 0) return {};
    auto result = BoxCombineResult();
    auto pluses_zero_reference = std::vector<double>();
    pluses_zero_reference.push_back(-boxes.front().height);
    for (int i = 0; i < ssize(boxes); ++i) {
        auto& box = boxes[i];
        pluses_zero_reference.back() += box.height;
        pluses_zero_reference.push_back(box.depth);
        result.new_box.width = std::max(result.new_box.width, box.width);
        if (i <= reference_box) {
            result.new_box.height += box.height;
        }
        if (i < reference_box) {
            result.new_box.height += box.depth;
        }
        if (i >= reference_box) {
            result.new_box.depth += box.depth;
        }
        if (i > reference_box) {
            result.new_box.depth += box.height;
        }
    }
    for (int i = 0; i < ssize(boxes); ++i) {
        auto& pluses = result.pluses.emplace_back();
        pluses.second = pluses_zero_reference[reference_box]
            - pluses_zero_reference[i];
    }
    return result;
}
