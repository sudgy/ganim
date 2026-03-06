#ifndef GANIM_SCRIPT_COMPILE_STATEMENT_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_STATEMENT_EXPRESSION_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_expression_statement(
        Compiler& compiler,
        const syntax::ExprStatement& ast
    );
}

#endif
