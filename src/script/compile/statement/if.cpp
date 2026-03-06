#include "if.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/compile/statement/statement.hpp"
#include "script/script_exception.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

void compile_if_statement(
    Compiler& compiler,
    const syntax::IfStatement& ast
)
{
    auto condition = compile_expression(compiler, ast.condition);
    if (condition.type != Type::get_tag<bool>()) {
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
        auto true_branch = compiler.get_next_label();
        auto end_branch = compiler.get_next_label();
        compiler.write_jump(bytecode::jump_gt, true_branch);
        compiler.write_jump(end_branch);
        compiler.add_label_reference(true_branch);
        compiler.push_symbols();
        for (auto& statement : ast.true_statements) {
            compile_statement(compiler, statement);
        }
        compiler.write_pop(compiler.pop_symbols());
        compiler.add_label_reference(end_branch);
    }
    else if (not has_true_branch and has_false_branch) {
        auto false_branch = compiler.get_next_label();
        auto end_branch = compiler.get_next_label();
        compiler.write_jump(bytecode::jump_eq, false_branch);
        compiler.write_jump(end_branch);
        compiler.add_label_reference(false_branch);
        compiler.push_symbols();
        for (auto& statement : ast.false_statements) {
            compile_statement(compiler, statement);
        }
        compiler.write_pop(compiler.pop_symbols());
        compiler.add_label_reference(end_branch);
    }
    else if (has_true_branch  and has_false_branch) {
        auto true_branch = compiler.get_next_label();
        auto false_branch = compiler.get_next_label();
        auto end_branch = compiler.get_next_label();
        compiler.write_jump(bytecode::jump_gt, true_branch);
        compiler.write_jump(false_branch);

        compiler.add_label_reference(true_branch);
        compiler.push_symbols();
        for (auto& statement : ast.true_statements) {
            compile_statement(compiler, statement);
        }
        compiler.write_pop(compiler.pop_symbols());
        compiler.write_jump(end_branch);

        compiler.add_label_reference(false_branch);
        compiler.push_symbols();
        for (auto& statement : ast.false_statements) {
            compile_statement(compiler, statement);
        }
        compiler.write_pop(compiler.pop_symbols());

        compiler.add_label_reference(end_branch);
    }
    else {
        compiler.write_pop(1);
    }
}

}
