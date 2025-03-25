#include "script.hpp"

#include "script_exception.hpp"

using namespace ganim;

Script::Script(std::string script)
: M_script(std::move(script))
{
    M_tokens = tokenize(M_script);
    while (M_index < ssize(M_tokens)) {
        auto token = consume_token();
        auto it = G_command_factory.find(std::string(token.string));
        if (it == G_command_factory.end()) {
            throw std::runtime_error(
                    std::format("Unknown command \"{}\"", token.string));
        }
        M_commands.push_back(it->second(*this));
    }
}

void Script::execute() const
{
    for (auto& command : M_commands) {
        command->execute();
    }
}

const Token& Script::consume_token()
{
    if (M_index >= ssize(M_tokens)) {
        throw ScriptException(-1, -1, "Expected token");
    }
    auto& result = M_tokens[M_index];
    ++M_index;
    return result;
}

void Script::expect_semicolon()
{
    if (M_index >= ssize(M_tokens)) {
        throw ScriptException(-1, -1, "Expected semicolon");
    }
    auto& token = consume_token();
    if (token.string != ";") {
        throw ScriptException(
                token.line_number, token.column_number, "Expected semicolon");
    }
}
