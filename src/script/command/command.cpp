#include "command.hpp"

#include <utility>

#include "expression.hpp"
#include "declare_variable.hpp"
#include "set.hpp"
#include "if.hpp"
#include "while.hpp"
#include "break.hpp"
#include "continue.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

std::unique_ptr<Command> Command::from_ast(
    SymbolTable& table,
    const syntax::Statement& statement
)
{
    return std::visit(overloaded{
        [&](const syntax::ExprStatement& value) -> std::unique_ptr<Command>
        {
            return std::make_unique<commands::Expression>(
                Expression::from_ast(table, value.expression));
        },
        [&](const syntax::VarStatement& value) -> std::unique_ptr<Command>
        {
            return std::make_unique<commands::DeclareVariable>(table, value);
        },
        [&](const syntax::SetStatement& value) -> std::unique_ptr<Command>
        {
            return std::make_unique<commands::Set>(table, value);
        },
        [&](const syntax::IfStatement& value) -> std::unique_ptr<Command>
        {
            return std::make_unique<commands::If>(table, value);
        },
        [&](const syntax::WhileStatement& value) -> std::unique_ptr<Command>
        {
            return std::make_unique<commands::While>(table, value);
        },
        [&](const syntax::WordStatement& value) -> std::unique_ptr<Command>
        {
            if (value.statement == "break") {
                return std::make_unique<commands::Break>();
            }
            else if (value.statement == "continue") {
                return std::make_unique<commands::Continue>();
            }
            std::unreachable();
        }
    }, statement.value);
}
