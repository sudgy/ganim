#ifndef GANIM_SCRIPT_COMPILE_SET_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_SET_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_set_statement(
        Compiler& compiler,
        const syntax::SetStatement& ast
    );
}

#endif
