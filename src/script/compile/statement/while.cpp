#include "while.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/compile/statement/statement.hpp"
#include "script/script_exception.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

void compile_while_statement(
    Compiler& compiler,
    const syntax::WhileStatement& ast
)
{
    // loop_start:
    //      [condition]
    //      jg loop_body
    //      jump loop_end
    // loop_body:
    //      [body]
    //      [pop the stack]
    //      jump loop_start
    // loop_end:
    auto loop_start = compiler.get_next_label();
    auto loop_body = compiler.get_next_label();
    auto loop_end = compiler.get_next_label();

    compiler.add_label_reference(loop_start);
    auto condition = compile_expression(compiler, ast.condition);
    if (condition.type != any_pointer::get_tag<bool>()) {
        throw CompileError(
            ast.condition.line_number, ast.condition.column_number,
            "Expected boolean expression"
        );
    }
    compiler.write_jump(bytecode::jump_gt, loop_body);
    compiler.write_jump(loop_end);

    compiler.add_label_reference(loop_body);
    compiler.push_symbols();
    for (auto& statement : ast.loop_statements) {
        compile_statement(compiler, statement);
    }
    compiler.write_pop(compiler.pop_symbols());
    compiler.write_jump(loop_start);

    compiler.add_label_reference(loop_end);
}

}
