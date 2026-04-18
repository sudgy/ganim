#ifndef GANIM_SCRIPT_COMPILE_RETURN_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_RETURN_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_return_statement(
        Compiler& compiler,
        const syntax::ReturnStatement& ast
    );
}

#endif
