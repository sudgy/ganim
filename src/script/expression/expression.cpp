#include "expression.hpp"

#include "constant.hpp"
#include "variable.hpp"

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
    return std::visit(overloaded{
        [&](const syntax::Constant& ast) -> std::unique_ptr<Expression>
        {
            return expressions::constant_from_ast(ast);
        },
        [&](const syntax::Identifier& ast) -> std::unique_ptr<Expression>
        {
            return expressions::Variable::from_ast(script, ast);
        }
    }, ast.value);
}
