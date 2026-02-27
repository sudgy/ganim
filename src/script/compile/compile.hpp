#ifndef GANIM_SCRIPT_COMPILE_HPP
#define GANIM_SCRIPT_COMPILE_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    std::vector<std::byte> compile(const std::vector<syntax::Statement>& ast);
}

#endif
