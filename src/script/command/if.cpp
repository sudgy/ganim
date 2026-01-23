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
    M_statements.reserve(ast.statements.size());
    for (auto& statement : ast.statements) {
        M_statements.push_back(Command::from_ast(table, statement));
    }
}

void If::execute() const
{
    if (*M_condition->value().get_as<bool>()) {
        for (auto& statement : M_statements) {
            statement->execute();
        }
    }
}
