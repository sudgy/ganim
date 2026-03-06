#ifndef GANIM_SCRIPT_COMPILE_DECLARE_WHILE_EXPRESSION_HPP
#define GANIM_SCRIPT_COMPILE_DECLARE_WHILE_EXPRESSION_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_while_statement(
        Compiler& compiler,
        const syntax::WhileStatement& ast
    );
}

#endif
