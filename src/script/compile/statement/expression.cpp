#include "expression.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/bytecode/bytecodes.hpp"

namespace ganim {

void compile_expression_statement(
    CompilerState& state,
    const syntax::ExprStatement& ast
)
{
    auto size = compile_expression(state, ast.expression).size();
    if (size != 0) {
        if (size % 8 == 0) size /= 8;
        else {
            size /= 8;
            ++size;
        }
        state.bytecode.push_back(bytecode::pop);
        state.write_parameter(size);
    }
}

}
