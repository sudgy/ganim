#ifndef GANIM_GEX_SPLIT_HPP
#define GANIM_GEX_SPLIT_HPP

#include <vector>

#include "section.hpp"

namespace ganim::gex {
    std::vector<Section> split(const TokenList& tokens);
}

#endif
