#ifndef GANIM_SCRIPT_COMPILE_FUNCTION_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_FUNCTION_EXPRESSION_HPP

#include "script/parse/expression.hpp"
#include "script/compile/value.hpp"

namespace ganim {
    class Compiler;
    Value compile_function_expression(
        Compiler& compiler,
        const syntax::FunctionExpression& ast
    );
}

#endif
