#include "script.hpp"

#include <unordered_set>

#include "script_exception.hpp"
#include "tokenize.hpp"

#include "ganimscript_parser.hpp"

#include "command/print.hpp"
#include "command/declare_variable.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

Script::Script(std::string script)
: M_script(std::move(script)) {}

void Script::compile()
{
    M_commands.clear();
    auto tokens = tokenize(M_script);
    if (tokens.empty()) return;
    auto parser = syntax::ScriptParser();
    auto token_index = 0;
    while (!parser.finished()) {
        try {
            if (token_index == ssize(tokens)) {
                parser.push(parser.end_token());
                break;
            }
        }
        catch (std::invalid_argument&) {
            throw CompileError(-1, -1, "Syntax error");
        }
        auto& token = tokens[token_index];
        try {
            bool pushed = false;
            switch (token.type) {
            case Token::Int:
                pushed = parser.push(parser.Int_token(
                    {std::stol(std::string(token.string)),
                    token.line_number, token.column_number}));
                break;
            case Token::Double:
                pushed = parser.push(parser.Double_token(
                    {std::stod(std::string(token.string)),
                    token.line_number, token.column_number}));
                break;
            case Token::String:
                pushed = parser.push(parser.String_token(
                    {token.string, token.line_number, token.column_number}));
                break;
            case Token::Else:
                pushed = parser.push(parser.builtin_token(token.string));
                break;
            case Token::Identifier:
                auto keywords = std::unordered_set<std::string_view>{
                    "print",
                    "var"
                };
                if (keywords.contains(token.string)) {
                    pushed = parser.push(parser.builtin_token(token.string));
                }
                else if (token.string == "true") {
                    pushed = parser.push(parser.Boolean_token(
                        {true, token.line_number, token.column_number}));
                }
                else if (token.string == "false") {
                    pushed = parser.push(parser.Boolean_token(
                        {false, token.line_number, token.column_number}));
                }
                else {
                    pushed = parser.push(parser.Identifier_token(
                       {token.string, token.line_number, token.column_number}));
                }
                break;
            }
            if (!pushed) throw CompileError(-1, -1, "Syntax error");
        }
        catch (std::invalid_argument&) {
            throw CompileError(
                token.line_number, token.column_number, "Syntax error");
        }
        ++token_index;
    }
    auto& statements = parser.get_result();
    M_commands.reserve(statements.size());
    for (auto& statement : statements) {
        std::visit(overloaded{
            [&](const syntax::PrintStatement& value)
            {
                M_commands.push_back(std::make_unique<
                    commands::Print>(*this, value));
            },
            [&](const syntax::VarStatement& value)
            {
                M_commands.push_back(std::make_unique<
                    commands::DeclareVariable>(*this, value));
            }
        }, statement.value);
    }
}

void Script::execute()
{
    for (auto& command : M_commands) {
        command->execute();
    }
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
