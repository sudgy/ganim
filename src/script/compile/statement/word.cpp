#include "word.hpp"

#include <utility>

#include "script/compile/compiler.hpp"
#include "script/script_exception.hpp"

void ganim::compile_word_statement(
    Compiler& compiler,
    const syntax::WordStatement& ast
)
{
    if (ast.statement == "break") {
        auto break_label = compiler.get_break_label();
        if (!break_label) {
            throw CompileError(
                ast.line_number, ast.column_number,
                "break outside of a loop"
            );
        }
        compiler.write_pop(compiler.get_loop_pop_size());
        compiler.write_jump(*break_label);
    }
    else if (ast.statement == "continue") {
        auto continue_label = compiler.get_continue_label();
        if (!continue_label) {
            throw CompileError(
                ast.line_number, ast.column_number,
                "continue outside of a loop"
            );
        }
        compiler.write_pop(compiler.get_loop_pop_size());
        compiler.write_jump(*continue_label);
    }
    else std::unreachable();
}
