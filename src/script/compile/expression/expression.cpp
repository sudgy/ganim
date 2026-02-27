#include "expression.hpp"

#include "overloaded.hpp"

#include "function.hpp"

namespace ganim {

void compile_expression(CompilerState& state, const syntax::Expression& ast)
{
    std::visit(overloaded{
        [&](const syntax::FunctionExpression& value) {
            compile_function_expression(state, value);
        },
        [&](const auto&) {
            throw std::runtime_error("Unimplemented expression");
        }
    }, ast.value);
}

}
