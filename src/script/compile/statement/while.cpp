#include "while.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/compile/statement/statement.hpp"
#include "script/script_exception.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"

namespace ganim {

void compile_while_statement(
    CompilerState& state,
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
    auto loop_start = state.get_next_label();
    auto loop_body = state.get_next_label();
    auto loop_end = state.get_next_label();
    state.labels.emplace_back(state.bytecode.size(), loop_start);
    auto condition = compile_expression(state, ast.condition);
    if (condition.type != any_pointer::get_tag<bool>()) {
        throw CompileError(
            ast.condition.line_number, ast.condition.column_number,
            "Expected boolean expression"
        );
    }
    state.jumps.emplace_back(state.bytecode.size(), loop_body);
    state.bytecode.push_back(bytecode::jump_gt);
    state.bytecode.push_back(byte(0));
    state.jumps.emplace_back(state.bytecode.size(), loop_end);
    state.bytecode.push_back(bytecode::jump_long);
    for (int i = 0; i < 8; ++i) state.bytecode.push_back(byte(0));

    state.labels.emplace_back(state.bytecode.size(), loop_body);
    auto table = SymbolTable();
    state.symbols.push(table);
    for (auto& statement : ast.loop_statements) {
        compile_statement(state, statement);
    }
    auto pop_amount = state.symbols.pop();
    if (pop_amount > 0) {
        state.bytecode.push_back(bytecode::pop);
        state.write_parameter(pop_amount);
    }
    state.jumps.emplace_back(state.bytecode.size(), loop_start);
    state.bytecode.push_back(bytecode::jump_long);
    for (int i = 0; i < 8; ++i) state.bytecode.push_back(byte(0));

    state.labels.emplace_back(state.bytecode.size(), loop_end);
}

}
