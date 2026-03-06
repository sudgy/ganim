#ifndef GANIM_SCRIPT_COMPILE_BINARY_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_BINARY_EXPRESSION_HPP

#include "script/parse/expression.hpp"
#include "script/compile/value.hpp"

namespace ganim {
    class Compiler;
    Value compile_binary_expression(
        Compiler& compiler,
        const syntax::BinaryExpression& ast
    );
}

#endif
