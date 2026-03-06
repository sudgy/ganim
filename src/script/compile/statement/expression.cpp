#include "expression.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

void compile_expression_statement(
    Compiler& compiler,
    const syntax::ExprStatement& ast
)
{
    auto size = compile_expression(compiler, ast.expression).type.size();
    if (size != 0) {
        if (size % 8 == 0) size /= 8;
        else {
            size /= 8;
            ++size;
        }
        compiler.write_pop(size);
    }
}

}
