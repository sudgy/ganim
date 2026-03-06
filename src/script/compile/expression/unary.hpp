#ifndef GANIM_SCRIPT_COMPILE_UNARY_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_UNARY_EXPRESSION_HPP

#include "script/parse/expression.hpp"
#include "script/compile/value.hpp"

namespace ganim {
    class Compiler;
    Value compile_unary_expression(
        Compiler& compiler,
        const syntax::UnaryExpression& ast
    );
}

#endif
