#ifndef GANIM_SCRIPT_COMPILE_DECLARE_VARIABLE_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_DECLARE_VARIABLE_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/statement.hpp"

namespace ganim {
    void compile_variable_statement(
        CompilerState& state,
        const syntax::VarStatement& ast
    );
}

#endif
