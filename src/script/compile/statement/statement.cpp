#include "statement.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "if.hpp"
#include "set.hpp"
#include "variable.hpp"
#include "while.hpp"
#include "word.hpp"

namespace ganim {

void compile_statement(Compiler& compiler, const syntax::Statement& ast)
{
    std::visit(overloaded{
        [&](const syntax::ExprStatement& value) {
            compile_expression_statement(compiler, value);
        },
        [&](const syntax::IfStatement& value) {
            compile_if_statement(compiler, value);
        },
        [&](const syntax::SetStatement& value) {
            compile_set_statement(compiler, value);
        },
        [&](const syntax::VarStatement& value) {
            compile_variable_statement(compiler, value);
        },
        [&](const syntax::WhileStatement& value) {
            compile_while_statement(compiler, value);
        },
        [&](const syntax::WordStatement& value) {
            compile_word_statement(compiler, value);
        },
        [&](const auto&) {
            throw std::runtime_error("Unimplemented statement");
        }
    }, ast.value);
}

}
