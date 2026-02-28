#ifndef GANIM_SCRIPT_COMPILE_CONSTANT_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_CONSTANT_EXPRESSION_HPP

#include "script/compile/compiler_state.hpp"
#include "script/parse/expression.hpp"
#include "script/type.hpp"

namespace ganim {
    Type compile_constant_expression(
        CompilerState& state,
        const syntax::ConstantExpression& ast
    );
}

#endif
