#ifndef GANIM_GEX_PREPROCESS_HPP
#define GANIM_GEX_PREPROCESS_HPP

#include <vector>

#include "token.hpp"

namespace ganim::gex {
    TokenList preprocess(bool math, const std::vector<std::string_view>& input);
}

#endif
