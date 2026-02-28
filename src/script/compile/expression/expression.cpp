#include "expression.hpp"

#include "overloaded.hpp"

#include "constant.hpp"
#include "function.hpp"
#include "unary.hpp"

namespace ganim {

Type compile_expression(CompilerState& state, const syntax::Expression& ast)
{
    return std::visit(overloaded{
        [&](const syntax::ConstantExpression& value) -> Type {
            return compile_constant_expression(state, value);
        },
        [&](const syntax::FunctionExpression& value) -> Type {
            return compile_function_expression(state, value);
        },
        [&](const syntax::UnaryExpression& value) -> Type {
            return compile_unary_expression(state, value);
        },
        [&](const auto&) -> Type {
            throw std::runtime_error("Unimplemented expression");
        }
    }, ast.value);
}

}
