#ifndef GANIM_SCRIPT_COMPILE_IF_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_IF_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_if_statement(
        Compiler& compiler,
        const syntax::IfStatement& ast
    );
}

#endif
