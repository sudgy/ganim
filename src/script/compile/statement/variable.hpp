#ifndef GANIM_SCRIPT_COMPILE_DECLARE_VARIABLE_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_DECLARE_VARIABLE_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_variable_statement(
        Compiler& compiler,
        const syntax::VarStatement& ast
    );
}

#endif
