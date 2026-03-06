#ifndef GANIM_SCRIPT_COMPILE_VARIABLE_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_VARIABLE_EXPRESSION_HPP

#include "script/parse/expression.hpp"
#include "script/compile/value.hpp"

namespace ganim {
    class Compiler;
    Value compile_variable_expression(
        Compiler& compiler,
        const syntax::IdentifierExpression& ast
    );
}

#endif
