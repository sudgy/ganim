#ifndef GANIM_GEX_SECTION_RENDER_HPP
#define GANIM_GEX_SECTION_RENDER_HPP

#include "section.hpp"
#include "style.hpp"

namespace ganim::gex {
    Box section_render(
        Section section,
        int pixel_size,
        Style style = Style::Text
    );
}

#endif
