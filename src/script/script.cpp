#include "script.hpp"

#include <unordered_set>

#include "script_exception.hpp"
#include "tokenize.hpp"

#include "ganimscript_parser.hpp"

#include "command/expression.hpp"
#include "command/declare_variable.hpp"

#include "function/binary.hpp"
#include "function/unary.hpp"
#include "function/print.hpp"
#include "function/equality.hpp"
#include "function/comparison.hpp"
#include "function/boolean.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

Script::Script(std::string script)
: M_script(std::move(script))
{
    add_function("__plus__", std::make_unique<functions::Add<std::int64_t>>());
    add_function("__plus__", std::make_unique<functions::Add<double>>());
    add_function("__plus__", std::make_unique<functions::Add<std::string>>());
    add_function("__minus__", std::make_unique<functions::Subtract<std::int64_t>>());
    add_function("__minus__", std::make_unique<functions::Subtract<double>>());
    add_function("__mult__", std::make_unique<functions::Multiply<std::int64_t>>());
    add_function("__mult__", std::make_unique<functions::Multiply<double>>());
    add_function("__div__", std::make_unique<functions::Divide<std::int64_t>>());
    add_function("__div__", std::make_unique<functions::Divide<double>>());
    add_function("__mod__", std::make_unique<functions::Modulo<std::int64_t>>());

    add_function("__unary_plus__",
                 std::make_unique<functions::UnaryPlus<std::int64_t>>());
    add_function("__unary_plus__",
                 std::make_unique<functions::UnaryPlus<double>>());
    add_function("__unary_minus__",
                 std::make_unique<functions::UnaryMinus<std::int64_t>>());
    add_function("__unary_minus__",
                 std::make_unique<functions::UnaryMinus<double>>());

    add_function("__eq__", std::make_unique<functions::Equal<std::int64_t>>());
    add_function("__eq__", std::make_unique<functions::Equal<double>>());
    add_function("__eq__", std::make_unique<functions::Equal<bool>>());
    add_function("__eq__", std::make_unique<functions::Equal<std::string>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<std::int64_t>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<double>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<bool>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<std::string>>());

    add_function("__lt__",
                 std::make_unique<functions::LessThan<std::int64_t>>());
    add_function("__lt__",
                 std::make_unique<functions::LessThan<double>>());
    add_function("__le__",
                 std::make_unique<functions::LessThanOrEqual<std::int64_t>>());
    add_function("__le__",
                 std::make_unique<functions::LessThanOrEqual<double>>());
    add_function("__gt__",
                 std::make_unique<functions::GreaterThan<std::int64_t>>());
    add_function("__gt__",
                 std::make_unique<functions::GreaterThan<double>>());
    add_function("__ge__",
                 std::make_unique<functions::GreaterThanOrEqual<std::int64_t>>());
    add_function("__ge__",
                 std::make_unique<functions::GreaterThanOrEqual<double>>());

    add_function("__and__", std::make_unique<functions::And>());
    add_function("__or__", std::make_unique<functions::Or>());
    add_function("__xor__", std::make_unique<functions::Xor>());
    add_function("__nand__", std::make_unique<functions::Nand>());
    add_function("__nor__", std::make_unique<functions::Nor>());
    add_function("__not__", std::make_unique<functions::Not>());

    add_function("print", std::make_unique<functions::PrintInt>());
    add_function("print", std::make_unique<functions::PrintDouble>());
    add_function("print", std::make_unique<functions::PrintString>());
    add_function("print", std::make_unique<functions::PrintBool>());
}

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
                    "var",
                    "and",
                    "not",
                    "or",
                    "xor",
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
        std::visit(overloaded{
            [&](const syntax::ExprStatement& value)
            {
                M_commands.push_back(std::make_unique<
                    commands::Expression>(
                        Expression::from_ast(*this, value.expression)));
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

void Script::add_function(
    std::string_view name,
    std::unique_ptr<Function> function,
    int line_number,
    int column_number
)
{
    auto name_string = std::string(name);
    auto& functions = M_functions[name_string];
    for (auto& f : functions) {
        if (f->get_result_type() == function->get_result_type() and
            f->get_input_types() == function->get_input_types())
        {
            throw CompileError(line_number, column_number, std::format(
                    "A function by the name \"{}\" already exists.", name));
        }
    }
    functions.emplace_back(std::move(function));
}

Value* Script::get_variable(const std::string& name)
{
    auto it = M_variables.find(name);
    if (it == M_variables.end()) return nullptr;
    else return it->second.get();
}

std::vector<Function*> Script::get_functions(const std::string& name)
{
    auto& results = M_functions[name];
    auto result = std::vector<Function*>();
    result.resize(results.size());
    for (int i = 0; i < ssize(results); ++i) {
        result[i] = results[i].get();
    }
    return result;
}
