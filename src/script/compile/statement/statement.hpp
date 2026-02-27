#ifndef GANIM_SCRIPT_COMPILE_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_STATEMENT_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/statement.hpp"

namespace ganim {
    void compile_statement(CompilerState& state, const syntax::Statement& ast);
}

#endif
