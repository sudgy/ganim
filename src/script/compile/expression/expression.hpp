#ifndef GANIM_SCRIPT_COMPILE_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_EXPRESSION_HPP

#include "script/parse/expression.hpp"
#include "script/compile/value.hpp"

namespace ganim {
    class Compiler;
    Value compile_expression(
        Compiler& compiler,
        const syntax::Expression& ast
    );
}

#endif
