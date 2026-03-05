#include "variable.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/script_exception.hpp"

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
    if (ast.type) {
        if (state.symbols.get_type(*ast.type) != value.type) {
            throw CompileError(
                ast.type->name.line_number,
                ast.type->name.column_number,
                "Type specifier does not match expression"
            );
        }
    }
    state.symbols.add_variable(
        name.name,
        value.type,
        !ast.constant,
        name.line_number,
        name.column_number
    );
}

}
