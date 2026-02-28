#include "unary.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/script_exception.hpp"

namespace ganim {

Type compile_unary_expression(
    CompilerState& state,
    const syntax::UnaryExpression& ast
)
{
    auto type = compile_expression(state, *ast.subexpression);
    switch (ast.op) {
        case syntax::UnaryExpression::Plus:
            break;
        case syntax::UnaryExpression::Minus:
            if (type == any_pointer::get_tag<int64_t>()) {
                state.bytecode.push_back(bytecode::unary_minus_int);
            }
            else if (type == any_pointer::get_tag<double>()) {
                state.bytecode.push_back(bytecode::unary_minus_double);
            }
            else {
                throw CompileError(ast.line_number, ast.column_number,
                    "Unable to find this operation for this type");
            }
            break;
        default:
            throw std::runtime_error("Not yet implemented");
    }
    return type;
}

}
