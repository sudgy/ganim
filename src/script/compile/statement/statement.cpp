#include "statement.hpp"

#include "overloaded.hpp"

#include "expression.hpp"

namespace ganim {

void compile_statement(CompilerState& state, const syntax::Statement& ast)
{
    std::visit(overloaded{
        [&](const syntax::ExprStatement& value) {
            compile_expression_statement(state, value);
        },
        [&](const auto&) {
            throw std::runtime_error("Unimplemented statement");
        }
    }, ast.value);
}

}
