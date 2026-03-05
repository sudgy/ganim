#ifndef GANIM_SCRIPT_COMPILE_DECLARE_IF_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_DECLARE_IF_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/statement.hpp"

namespace ganim {
    void compile_if_statement(
        CompilerState& state,
        const syntax::IfStatement& ast
    );
}

#endif
