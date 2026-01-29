#include "loop.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

Loop::Loop(SymbolTable& table, const syntax::LoopStatement& ast)
{
    M_loop_commands.reserve(ast.loop_statements.size());
    table.push(M_loop_table);
    for (auto& statement : ast.loop_statements) {
        M_loop_commands.push_back(Command::from_ast(table, statement));
    }
    table.pop();
}

Command::ExecuteResult Loop::execute() const
{
    auto result = ExecuteResult::Nothing;
    while (true) {
        for (auto& command : M_loop_commands) {
            result = command->execute();
            if (result != Nothing) break;
        }
        if (result == Break) break;
    }
    return Nothing;
}
