#ifndef GANIM_SCRIPT_COMPILE_BINARY_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_BINARY_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/expression.hpp"
#include "script/type.hpp"

namespace ganim {
    Type compile_binary_expression(
        CompilerState& state,
        const syntax::BinaryExpression& ast
    );
}

#endif
