#include "constant.hpp"

using namespace ganim;
using namespace ganim::expressions;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

std::unique_ptr<Expression> expressions::constant_from_ast(
    const syntax::ConstantExpression& ast
)
{
    return visit(overloaded{
        [&](std::int64_t value) -> std::unique_ptr<Expression>
        {
            return std::make_unique<Constant<std::int64_t>>(
                value, ast.line_number, ast.column_number);
        },
        [&](double value) -> std::unique_ptr<Expression>
        {
            return std::make_unique<Constant<double>>(
                value, ast.line_number, ast.column_number);
        },
        [&](bool value) -> std::unique_ptr<Expression>
        {
            return std::make_unique<Constant<bool>>(
                value, ast.line_number, ast.column_number);
        },
        [&](std::string_view value) -> std::unique_ptr<Expression>
        {
            return std::make_unique<Constant<std::string>>(
                std::string(value), ast.line_number, ast.column_number);
        },
    }, ast.value);
}
