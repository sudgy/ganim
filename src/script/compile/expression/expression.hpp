#ifndef GANIM_SCRIPT_COMPILE_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/expression.hpp"
#include "script/compile/value.hpp"

namespace ganim {
    Value compile_expression(
        CompilerState& state,
        const syntax::Expression& ast
    );
}

#endif
