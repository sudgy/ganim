#include "while.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

While::While(SymbolTable& table, const syntax::WhileStatement& ast)
{
    M_condition = Expression::from_ast(table, ast.condition);
    if (M_condition->type() != Type(any_pointer::get_tag<bool>())) {
        throw CompileError(
            ast.condition.line_number, ast.condition.column_number,
            "Expected boolean expression"
        );
    }
    M_loop_commands.reserve(ast.loop_statements.size());
    table.push(M_loop_table);
    for (auto& statement : ast.loop_statements) {
        M_loop_commands.push_back(Command::from_ast(table, statement));
    }
    table.pop();
}

Command::ExecuteResult While::execute() const
{
    auto result = ExecuteResult::Nothing;
    while (*M_condition->value().get_as<bool>()) {
        for (auto& command : M_loop_commands) {
            result = command->execute();
            if (result != Nothing) break;
        }
        if (result == Break) break;
    }
    return Nothing;
}
