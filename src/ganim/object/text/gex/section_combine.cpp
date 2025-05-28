#include "section_combine.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<PositionedGlyph> gex::section_combine(
    const std::vector<RenderedSection>& sections
)
{
    return sections[0].glyphs;
}
