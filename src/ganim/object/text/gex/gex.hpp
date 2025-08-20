#ifndef GANIM_GEX_HPP
#define GANIM_GEX_HPP

#include "ganim/object/text/text_helpers.hpp"

namespace ganim {
    std::vector<PositionedGlyph>
    gex_render(bool math, const std::vector<std::string_view>& input);
}

#endif
