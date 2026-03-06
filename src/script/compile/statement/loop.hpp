#ifndef GANIM_SCRIPT_COMPILE_LOOP_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_LOOP_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_loop_statement(
        Compiler& compiler,
        const syntax::LoopStatement& ast
    );
}

#endif
