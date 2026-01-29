#include "script.hpp"

#include <unordered_set>

#include "script_exception.hpp"
#include "tokenize.hpp"

#include "ganimscript_parser.hpp"

using namespace ganim;

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
            case Token::Decimal:
                pushed = parser.push(parser.Int_token(
                    {std::stol(std::string(token.string)),
                    token.line_number, token.column_number}));
                break;
            case Token::Hex:
                pushed = parser.push(parser.Int_token(
                    {std::stol(std::string(token.string.substr(2)),nullptr, 16),
                    token.line_number, token.column_number}));
                break;
            case Token::Octal:
                pushed = parser.push(parser.Int_token(
                    {std::stol(std::string(token.string), nullptr, 8),
                    token.line_number, token.column_number}));
                break;
            case Token::Binary:
                pushed = parser.push(parser.Int_token(
                    {std::stol(std::string(token.string.substr(2)), nullptr, 2),
                    token.line_number, token.column_number}));
                break;
            case Token::Double:
                pushed = parser.push(parser.Double_token(
                    {std::stod(std::string(token.string)),
                    token.line_number, token.column_number}));
                break;
            case Token::String:
                pushed = parser.push(parser.String_token(
                    {
                        modify_string_literal(token.string),
                        token.line_number,
                        token.column_number
                    }));
                break;
            case Token::Else:
                pushed = parser.push(parser.builtin_token(token.string));
                break;
            case Token::Identifier:
                auto keywords = std::unordered_set<std::string_view>{
                    "var",
                    "let",
                    "if",
                    "else",
                    "nor",
                    "nand"
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
        M_commands.push_back(Command::from_ast(M_table, statement));
    }
}

void Script::execute()
{
    for (auto& command : M_commands) {
        command->execute();
    }
}

std::string_view Script::modify_string_literal(std::string_view literal)
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
