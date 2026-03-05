#ifndef GANIM_SCRIPT_COMPILE_DECLARE_SET_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_DECLARE_SET_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/statement.hpp"

namespace ganim {
    void compile_set_statement(
        CompilerState& state,
        const syntax::SetStatement& ast
    );
}

#endif
