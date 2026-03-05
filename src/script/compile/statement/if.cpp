#include "if.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/compile/statement/statement.hpp"
#include "script/script_exception.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"

namespace ganim {

void compile_if_statement(
    CompilerState& state,
    const syntax::IfStatement& ast
)
{
    auto condition = compile_expression(state, ast.condition);
    if (condition.type != any_pointer::get_tag<bool>()) {
        throw CompileError(
            ast.condition.line_number, ast.condition.column_number,
            "Expected boolean expression"
        );
    }
    // True:
    // jg true_branch;
    // jump end_branch;
    // true_branch:
    //      ...
    // end_branch:
    //
    // False:
    // je false_branch;
    // jump end_branch;
    // false_branch:
    //      ...
    // end_branch:
    //
    // True/false:
    // jg true_branch
    // jump false_branch
    // true_branch:
    //      ...
    //      jump end_branch;
    // false_branch:
    //      ...
    // end_branch:
    auto has_true_branch = !ast.true_statements.empty();
    auto has_false_branch = !ast.false_statements.empty();
    if (has_true_branch and not has_false_branch) {
        auto true_branch = state.get_next_label();
        auto end_branch = state.get_next_label();
        state.jumps.emplace_back(state.bytecode.size(), true_branch);
        state.bytecode.push_back(bytecode::jump_gt);
        state.bytecode.push_back(byte(0));
        state.jumps.emplace_back(state.bytecode.size(), end_branch);
        state.bytecode.push_back(bytecode::jump_long);
        for (int i = 0; i < 8; ++i) state.bytecode.push_back(byte(0));
        state.labels.emplace_back(state.bytecode.size(), true_branch);
        for (auto& statement : ast.true_statements) {
            compile_statement(state, statement);
        }
        state.labels.emplace_back(state.bytecode.size(), end_branch);
    }
    else if (not has_true_branch and has_false_branch) {
        auto false_branch = state.get_next_label();
        auto end_branch = state.get_next_label();
        state.jumps.emplace_back(state.bytecode.size(), false_branch);
        state.bytecode.push_back(bytecode::jump_eq);
        state.bytecode.push_back(byte(0));
        state.jumps.emplace_back(state.bytecode.size(), end_branch);
        state.bytecode.push_back(bytecode::jump_long);
        for (int i = 0; i < 8; ++i) state.bytecode.push_back(byte(0));
        state.labels.emplace_back(state.bytecode.size(), false_branch);
        for (auto& statement : ast.false_statements) {
            compile_statement(state, statement);
        }
        state.labels.emplace_back(state.bytecode.size(), end_branch);
    }
    else if (has_true_branch  and has_false_branch) {
        auto true_branch = state.get_next_label();
        auto false_branch = state.get_next_label();
        auto end_branch = state.get_next_label();
        state.jumps.emplace_back(state.bytecode.size(), true_branch);
        state.bytecode.push_back(bytecode::jump_gt);
        state.bytecode.push_back(byte(0));
        state.jumps.emplace_back(state.bytecode.size(), false_branch);
        state.bytecode.push_back(bytecode::jump_long);
        for (int i = 0; i < 8; ++i) state.bytecode.push_back(byte(0));
        state.labels.emplace_back(state.bytecode.size(), true_branch);
        for (auto& statement : ast.true_statements) {
            compile_statement(state, statement);
        }
        state.jumps.emplace_back(state.bytecode.size(), end_branch);
        state.bytecode.push_back(bytecode::jump_long);
        for (int i = 0; i < 8; ++i) state.bytecode.push_back(byte(0));
        state.labels.emplace_back(state.bytecode.size(), false_branch);
        for (auto& statement : ast.false_statements) {
            compile_statement(state, statement);
        }
        state.labels.emplace_back(state.bytecode.size(), end_branch);
    }
    else {
        state.bytecode.push_back(bytecode::pop);
        state.bytecode.push_back(byte(1));
    }
}

}
