#include "section_combine.hpp"

#include "ganim/fmap.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<PositionedGlyph> gex::section_combine(
    const std::vector<Box>& sections
)
{
    return combine_boxes_horizontally(sections).glyphs;
}
