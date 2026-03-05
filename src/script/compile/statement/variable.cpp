#include "variable.hpp"

#include "script/compile/expression/expression.hpp"

namespace ganim {

void compile_variable_statement(
    CompilerState& state,
    const syntax::VarStatement& ast
)
{
    auto name = ast.variable;
    // Compiling the expression already puts it at the top of the stack, which
    // is where its new storage is
    auto value = compile_expression(state, ast.value);
    // Type check
    state.symbols.add_variable(
        name.name,
        value.type,
        !ast.constant,
        name.line_number,
        name.column_number
    );
}

}
