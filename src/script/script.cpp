#include "script.hpp"

#include <unordered_set>

#include "script_exception.hpp"
#include "parse/tokenize.hpp"
#include "parse/parse.hpp"

using namespace ganim;

Script::Script(std::string script)
: M_script(std::move(script))
{
    M_table.add_builtins();
}

void Script::compile()
{
    M_commands.clear();
    auto tokens = tokenize(M_script);
    if (tokens.empty()) return;
    auto statements = parse(tokens);
    M_commands.reserve(statements.size());
    for (auto& statement : statements) {
        M_commands.push_back(Command::from_ast(M_table, statement));
    }
}

void Script::execute()
{
    for (auto& command : M_commands) {
        command->execute();
    }
}

std::string Script::modify_string_literal(std::string_view literal)
{
    literal.remove_prefix(1);
    literal.remove_suffix(1);
    auto& result = M_modified_string_literals.emplace_back(literal);
    for (auto i = 0UZ; i < result.size(); ++i) {
        if (result[i] == '\\') {
            result.erase(i, 1);
        }
    }
    return result;
}
