#ifndef GANIM_SCRIPT_PARSE_PARSE_HPP
#define GANIM_SCRIPT_PARSE_PARSE_HPP

#include "statement.hpp"
#include "tokenize.hpp"

namespace ganim {
    std::vector<syntax::Statement> parse(const std::vector<Token>& tokens);
}

#endif
