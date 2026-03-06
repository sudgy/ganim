#include "expression.hpp"

#include "overloaded.hpp"

#include "binary.hpp"
#include "constant.hpp"
#include "function.hpp"
#include "unary.hpp"
#include "variable.hpp"

namespace ganim {

Value compile_expression(Compiler& compiler, const syntax::Expression& ast)
{
    return std::visit(overloaded{
        [&](const syntax::BinaryExpression& value) -> Value {
            return compile_binary_expression(compiler, value);
        },
        [&](const syntax::ConstantExpression& value) -> Value {
            return compile_constant_expression(compiler, value);
        },
        [&](const syntax::FunctionExpression& value) -> Value {
            return compile_function_expression(compiler, value);
        },
        [&](const syntax::UnaryExpression& value) -> Value {
            return compile_unary_expression(compiler, value);
        },
        [&](const syntax::IdentifierExpression& value) -> Value {
            return compile_variable_expression(compiler, value);
        },
        [&](const auto&) -> Value {
            throw std::runtime_error("Unimplemented expression");
        }
    }, ast.value);
}

}
