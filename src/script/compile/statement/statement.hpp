#ifndef GANIM_SCRIPT_COMPILE_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_statement(Compiler& compiler, const syntax::Statement& ast);
}

#endif
