#include "expression.hpp"

#include "constant.hpp"
#include "variable.hpp"
#include "binary.hpp"
#include "unary.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}
std::unique_ptr<Expression> Expression::from_ast(
    Script& script,
    const syntax::Expression& ast
)
{
    return expressions::Sum::from_ast(script, ast);
}

std::unique_ptr<Expression> Expression::from_ast(
    Script& script,
    const syntax::Factor& ast
)
{
    return std::visit(overloaded{
        [&](const syntax::Constant& ast) -> std::unique_ptr<Expression>
        {
            return expressions::constant_from_ast(ast);
        },
        [&](const syntax::Identifier& ast) -> std::unique_ptr<Expression>
        {
            return expressions::Variable::from_ast(script, ast);
        },
        [&](const std::unique_ptr<syntax::Factor>& factor)
            -> std::unique_ptr<Expression>
        {
            if (ast.plus_sign) {
                return expressions::UnaryPlus::from_ast(script, *factor);
            }
            else {
                return expressions::UnaryMinus::from_ast(script, *factor);
            }
        },
        [&](const std::unique_ptr<syntax::Expression>& ast)
            -> std::unique_ptr<Expression>
        {
            return expressions::Sum::from_ast(script, *ast);
        }
    }, ast.value);
}
