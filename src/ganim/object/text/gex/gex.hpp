#ifndef GANIM_GEX_HPP
#define GANIM_GEX_HPP

#include "ganim/object/text/text_helpers.hpp"

namespace ganim {
    std::vector<PositionedGlyph>
    gex_render(const std::vector<std::string_view>& input);
}

#endif
