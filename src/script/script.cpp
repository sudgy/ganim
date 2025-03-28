#include "script.hpp"

#include "script_exception.hpp"

#include "script/expression/constant.hpp"
#include "script/expression/variable.hpp"

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
            throw CompileError(token.line_number, token.column_number,
                    std::format("Unknown command \"{}\"", token.string));
        }
        M_commands.push_back(it->second(*this));
        expect_semicolon();
    }
}

void Script::execute()
{
    for (auto& command : M_commands) {
        command->execute();
    }
}

const Token& Script::get_token()
{
    if (M_index >= ssize(M_tokens)) {
        throw CompileError(-1, -1, "Expected token");
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
        throw CompileError(-1, -1, "Expected semicolon");
    }
    auto& token = consume_token();
    if (token.string != ";") {
        throw CompileError(
                token.line_number, token.column_number, "Expected semicolon");
    }
}

std::unique_ptr<Expression> Script::get_expression()
{
    auto& token = consume_token();
    auto c = token.string[0];
    if (token.is_identifier) {
        if (token.string == "true") {
            return std::make_unique<expressions::Constant<bool>>(
                true,
                token.line_number,
                token.column_number
            );
        }
        else if (token.string == "false") {
            return std::make_unique<expressions::Constant<bool>>(
                false,
                token.line_number,
                token.column_number
            );
        }
        else {
            auto it = M_variables.find(std::string(token.string));
            if (it == M_variables.end()) {
                throw CompileError(token.line_number, token.column_number,
                        std::format("Unknown identifier \"{}\"", token.string));
            }
            return std::make_unique<expressions::Variable>(
                *it->second,
                token.line_number,
                token.column_number
            );
        }
    }
    else if (c == '"') {
        return std::make_unique<expressions::Constant<std::string>>(
            std::string(token.string.substr(1, token.string.size() - 2)),
            token.line_number,
            token.column_number
        );
    }
    else if ('0' <= c and c <= '9') {
        for (auto c : token.string) {
            if (!('0' <= c and c <= '9')) {
                return std::make_unique<expressions::Constant<double>>(
                    std::stod(std::string(token.string)),
                    token.line_number,
                    token.column_number
                );
            }
        }
        return std::make_unique<expressions::Constant<std::int64_t>>(
            std::stoi(std::string(token.string)),
            token.line_number,
            token.column_number
        );
    }
    else if (c == '.' and token.string.size() > 1) {
        return std::make_unique<expressions::Constant<double>>(
            std::stod(std::string(token.string)),
            token.line_number,
            token.column_number
        );
    }
    throw CompileError(token.line_number, token.column_number,
            "Expected expression");
}

void Script::add_variable(
    std::string_view name,
    std::unique_ptr<Value> variable,
    int line_number,
    int column_number
)
{
    auto name_string = std::string(name);
    if (M_variables.contains(name_string)) {
        throw CompileError(line_number, column_number, std::format(
                "A variable by the name \"{}\" already exists.", name));
    }
    M_variables[name_string] = std::move(variable);
}

Value* Script::get_variable(const std::string& name)
{
    auto it = M_variables.find(name);
    if (it == M_variables.end()) return nullptr;
    else return it->second.get();
}
