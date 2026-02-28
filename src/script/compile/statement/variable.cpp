#include "variable.hpp"

//#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
//#include "script/bytecode/bytecodes.hpp"

namespace ganim {

void compile_variable_statement(
    CompilerState& state,
    const syntax::VarStatement& ast
)
{
    auto name = ast.variable;
    auto type = compile_expression(state, ast.value);
    // Type check
    state.symbols.add_variable(
        name.name,
        type,
        name.line_number,
        name.column_number
    );
}

}
