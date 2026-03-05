#include "statement.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "if.hpp"
#include "set.hpp"
#include "variable.hpp"

namespace ganim {

void compile_statement(CompilerState& state, const syntax::Statement& ast)
{
    std::visit(overloaded{
        [&](const syntax::ExprStatement& value) {
            compile_expression_statement(state, value);
        },
        [&](const syntax::IfStatement& value) {
            compile_if_statement(state, value);
        },
        [&](const syntax::SetStatement& value) {
            compile_set_statement(state, value);
        },
        [&](const syntax::VarStatement& value) {
            compile_variable_statement(state, value);
        },
        [&](const auto&) {
            throw std::runtime_error("Unimplemented statement");
        }
    }, ast.value);
}

}
