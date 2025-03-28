#include "script.hpp"

#include "script_exception.hpp"

#include "script/expression/constant.hpp"
#include "script/expression/variable.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

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
    return combine_expression_pieces(get_expression_pieces());
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

std::vector<Script::Piece> Script::get_expression_pieces()
{
    auto result = std::vector<Piece>();
    while (true) {
        auto token = get_token();
        if (token.string == ")" or token.string == ";") {
            break;
        }
        auto c = token.string[0];
        ++M_index;
        if (token.is_identifier) {
            if (token.string == "true" or token.string == "false") {
                result.emplace_back(
                    token.line_number,
                    token.column_number,
                    Literal{token.string}
                );
            }
            else {
                result.emplace_back(
                    token.line_number,
                    token.column_number,
                    Identifier{token.string}
                );
            }
        }
        else if (c == '"') {
            result.emplace_back(
                token.line_number,
                token.column_number,
                Literal{token.string}
            );
        }
        else if ('0' <= c and c <= '9') {
            result.emplace_back(
                token.line_number,
                token.column_number,
                Literal{token.string}
            );
        }
        else if (c == '.' and token.string.size() > 1) {
            result.emplace_back(
                token.line_number,
                token.column_number,
                Literal{token.string}
            );
        }
        else if (token.string == "(") {
            auto sub_pieces = get_expression_pieces();
            auto end_token = get_token();
            if (end_token.string != ")") {
                throw CompileError(
                    end_token.line_number,
                    end_token.column_number,
                    "Expected ')'"
                );
            }
            result.emplace_back(
                token.line_number,
                token.column_number,
                Compound{std::move(sub_pieces)}
            );
            ++M_index;
        }
        else {
            int precedence = 0;
            if (token.string == "*") {
                precedence = 1;
            }
            else if (token.string == "+") {
                precedence = 2;
            }
            result.emplace_back(
                token.line_number,
                token.column_number,
                Operator{precedence, token.string}
            );
        }
    }
    return result;
}

std::unique_ptr<Expression> Script::combine_expression_pieces(
        std::vector<Piece> pieces)
{
    if (pieces.size() == 0) {
        auto token = get_token();
        throw CompileError(token.line_number, token.column_number,
                "Expected expression");
    }
    for (int precedence = 1; precedence <= 2; ++precedence) {
        for (int i = 0; i < ssize(pieces); ++i) {
            auto& piece = pieces[i];
            if (auto op = std::get_if<Operator>(&piece.data)) {
                if (op->precedence == precedence) {
                    handle_operator_piece(*op, pieces, i);
                }
            }
        }
    }
    if (pieces.size() > 1) {
        throw CompileError(pieces[1].line_number, pieces[1].column_number,
                "Unexpected expression");
    }
    if (pieces.size() == 0) {
        throw std::runtime_error("sudgy, you done goofed");
    }
    return convert_single_piece(pieces[0]);
}

void Script::handle_operator_piece(
    Operator& op,
    std::vector<Piece>& pieces,
    int& index
)
{
    (void)op;
    (void)pieces;
    (void)index;
}

std::unique_ptr<Expression> Script::convert_single_piece(Piece& piece)
{
    return std::visit(overloaded{
        [&](Identifier& identifier) -> std::unique_ptr<Expression>
        {
            auto it = M_variables.find(std::string(identifier.identifier));
            if (it == M_variables.end()) {
                throw CompileError(
                    piece.line_number,
                    piece.column_number,
                    std::format("Unknown identifier \"{}\"",
                        identifier.identifier)
                );
            }
            return std::make_unique<expressions::Variable>(
                *it->second,
                piece.line_number,
                piece.column_number
            );
        },
        [&](Literal& literal) -> std::unique_ptr<Expression>
        {
            if (literal.token == "true") {
                return std::make_unique<expressions::Constant<bool>>(
                    true,
                    piece.line_number,
                    piece.column_number
                );
            }
            else if (literal.token == "false") {
                return std::make_unique<expressions::Constant<bool>>(
                    false,
                    piece.line_number,
                    piece.column_number
                );
            }
            else if (literal.token[0] == '"') {
                return std::make_unique<expressions::Constant<std::string>>(
                    std::string(literal.token.substr(
                            1, literal.token.size() - 2)),
                    piece.line_number,
                    piece.column_number
                );
            }
            // Numeric
            else {
                for (auto c : literal.token) {
                    if (!('0' <= c and c <= '9')) {
                        return std::make_unique<expressions::Constant<double>>(
                            std::stod(std::string(literal.token)),
                            piece.line_number,
                            piece.column_number
                        );
                    }
                }
                return std::make_unique<expressions::Constant<std::int64_t>>(
                    std::stoi(std::string(literal.token)),
                    piece.line_number,
                    piece.column_number
                );
            }
        },
        [&](Operator&) -> std::unique_ptr<Expression>
        {
            throw CompileError(piece.line_number, piece.column_number,
                    "Expected expression");
        },
        [&](Compound& compound)
        {
            return combine_expression_pieces(std::move(compound.pieces));
        },
        [&](Processed& processed)
        {
            return std::move(processed.expression);
        }
    }, piece.data);
}
