#include "if.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

If::If(SymbolTable& table, const syntax::IfStatement& ast)
{
    M_condition = Expression::from_ast(table, ast.condition);
    if (M_condition->type() != Type(any_pointer::get_tag<bool>())) {
        throw CompileError(
            ast.condition.line_number, ast.condition.column_number,
            "Expected boolean expression"
        );
    }
    M_true_commands.reserve(ast.true_statements.size());
    table.push(M_true_table);
    for (auto& statement : ast.true_statements) {
        M_true_commands.push_back(Command::from_ast(table, statement));
    }
    table.pop();
    table.push(M_false_table);
    M_false_commands.reserve(ast.false_statements.size());
    for (auto& statement : ast.false_statements) {
        M_false_commands.push_back(Command::from_ast(table, statement));
    }
    table.pop();
}

void If::execute() const
{
    if (*M_condition->value().get_as<bool>()) {
        for (auto& command : M_true_commands) {
            command->execute();
        }
    }
    else {
        for (auto& command : M_false_commands) {
            command->execute();
        }
    }
}
