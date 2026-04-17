#include "statement.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "function.hpp"
#include "if.hpp"
#include "loop.hpp"
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
        [&](const syntax::LoopStatement& value) {
            compile_loop_statement(compiler, value);
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
        [&](const syntax::FunctionStatement& value) {
            compile_function_statement(compiler, value);
        }
    }, ast.value);
}

}
