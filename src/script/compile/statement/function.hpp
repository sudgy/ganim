#ifndef GANIM_SCRIPT_COMPILE_FUNCTION_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_FUNCTION_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_function_statement(
        Compiler& compiler,
        const syntax::FunctionStatement& ast
    );
}

#endif
