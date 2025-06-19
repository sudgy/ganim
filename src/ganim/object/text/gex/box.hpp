#ifndef GANIM_GEX_BOX_HPP
#define GANIM_GEX_BOX_HPP

#include <vector>

#include "ganim/object/text/text_helpers.hpp"

namespace ganim::gex {
    struct Box {
        double width = 0;
        double height = 0;
        double depth = 0;
        std::vector<PositionedGlyph> glyphs;
    };
    Box combine_boxes_horizontally(const std::vector<Box>& boxes);
    Box combine_boxes_vertically(
        const std::vector<Box>& boxes,
        int reference_box
    );
    Box box_from_glyphs(std::vector<PositionedGlyph> glyphs);
    void scale_box(Box& box, double scale);
}

#endif
