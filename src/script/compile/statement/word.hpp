#ifndef GANIM_SCRIPT_COMPILE_WORD_STATEMENT_HPP
#define GANIM_SCRIPT_COMPILE_WORD_STATEMENT_HPP

#include "script/parse/statement.hpp"

namespace ganim {
    class Compiler;
    void compile_word_statement(
        Compiler& compiler,
        const syntax::WordStatement& ast
    );
}

#endif
