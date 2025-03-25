#include "script.hpp"

#include "script_exception.hpp"

#include "script/expression/string_constant.hpp"

using namespace ganim;

Script::Script(std::string script)
: M_script(std::move(script)) {}

void Script::compile()
{
    M_index = 0;
    M_commands.clear();
    M_tokens = tokenize(M_script);
    while (M_index < ssize(M_tokens)) {
        auto token = consume_token();
        auto it = G_command_factory.find(std::string(token.string));
        if (it == G_command_factory.end()) {
            throw ScriptException(token.line_number, token.column_number,
                    std::format("Unknown command \"{}\"", token.string));
        }
        M_commands.push_back(it->second(*this));
        expect_semicolon();
    }
}

void Script::execute() const
{
    for (auto& command : M_commands) {
        command->execute();
    }
}

const Token& Script::get_token()
{
    if (M_index >= ssize(M_tokens)) {
        throw ScriptException(-1, -1, "Expected token");
    }
    auto& result = M_tokens[M_index];
    return result;
}

const Token& Script::consume_token()
{
    auto& result = get_token();
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

std::unique_ptr<Expression> Script::get_expression()
{
    auto& token = consume_token();
    if (token.string[0] == '"') {
        return std::make_unique<expressions::StringConstant>(
                std::string(token.string.substr(1, token.string.size() - 2)));
    }
    throw ScriptException(token.line_number, token.column_number,
            "Expected expression");
}
