#include "loop.hpp"

#include "script/compile/statement/statement.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

void compile_loop_statement(
    Compiler& compiler,
    const syntax::LoopStatement& ast
)
{
    auto loop_body = compiler.get_next_label();
    auto loop_end = compiler.get_next_label();

    compiler.add_label_reference(loop_body);
    compiler.push_symbols();
    compiler.set_loop_labels(loop_body, loop_end);
    for (auto& statement : ast.loop_statements) {
        compile_statement(compiler, statement);
    }
    compiler.write_pop(compiler.pop_symbols());
    compiler.write_jump(loop_body);

    compiler.add_label_reference(loop_end);
}

}
