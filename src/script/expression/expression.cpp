#include "expression.hpp"

#include "constant.hpp"
#include "variable.hpp"
#include "function.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

std::unique_ptr<Expression> Expression::from_ast(
    SymbolTable& table,
    const syntax::Expression& ast
)
{
    return std::visit(overloaded{
        [&](const syntax::ConstantExpression& ast) -> std::unique_ptr<Expression>
        {
            return expressions::constant_from_ast(ast);
        },
        [&](const syntax::IdentifierExpression& ast) -> std::unique_ptr<Expression>
        {
            return expressions::Variable::from_ast(table, ast);
        },
        [&](const syntax::FunctionExpression& ast) -> std::unique_ptr<Expression>
        {
            return expressions::Function::from_ast(table, ast);
        },
        [&](const syntax::UnaryExpression& ast)
            -> std::unique_ptr<Expression>
        {
            return expressions::Function::from_ast(table, ast);
        },
        [&](const syntax::BinaryExpression& ast)
            -> std::unique_ptr<Expression>
        {
            return expressions::Function::from_ast(table, ast);
        }
    }, ast.value);
}
