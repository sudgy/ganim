#ifndef GANIM_GEX_BOX_HPP
#define GANIM_GEX_BOX_HPP

#include <vector>

namespace ganim::gex {
    struct Box {
        double width = 0;
        double height = 0;
        double depth = 0;
    };
    struct BoxCombineResult {
        Box new_box;
        std::vector<std::pair<double, double>> pluses;
    };
    BoxCombineResult combine_boxes_horizontally(const std::vector<Box>& boxes);
    BoxCombineResult combine_boxes_vertically(
        const std::vector<Box>& boxes,
        int reference_box
    );
}

#endif
