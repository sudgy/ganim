#ifndef GANIM_SCRIPT_COMPILE_STATEMENT_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_STATEMENT_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/statement.hpp"

namespace ganim {
    void compile_expression_statement(
        CompilerState& state,
        const syntax::ExprStatement& ast
    );
}

#endif
