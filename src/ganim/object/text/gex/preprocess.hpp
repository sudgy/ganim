#ifndef GANIM_GEX_PREPROCESS_HPP
#define GANIM_GEX_PREPROCESS_HPP

#include <vector>

#include "token.hpp"

namespace ganim::gex {
    TokenList preprocess(const std::vector<std::string_view>& input);
}

#endif
