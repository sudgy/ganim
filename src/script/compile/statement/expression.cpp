#include "expression.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"

namespace ganim {

void compile_expression_statement(
    CompilerState& state,
    const syntax::ExprStatement& ast
)
{
    compile_expression(state, ast.expression);
}

}
