#ifndef GANIM_GEX_SECTION_COMBINE
#define GANIM_GEX_SECTION_COMBINE

#include "section.hpp"

namespace ganim::gex {
    std::vector<PositionedGlyph> section_combine(
        const std::vector<RenderedSection>& sections
    );
}

#endif
