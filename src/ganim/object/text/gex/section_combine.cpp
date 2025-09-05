#include "section_combine.hpp"

#include "ganim/fmap.hpp"

using namespace ganim;
using namespace ganim::gex;

Box gex::section_combine(const std::vector<Box>& sections)
{
    return combine_boxes_horizontally(sections, 0.1);
}
