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
    auto error = [&]{
        throw CompileError(ast.line_number, ast.column_number,
            "Unable to find this operation for this type");
    };
    switch (ast.op) {
        case syntax::UnaryExpression::Plus:
            if (type != any_pointer::get_tag<int64_t>() and
                type != any_pointer::get_tag<double>())
            {
                error();
            }
            break;
        case syntax::UnaryExpression::Minus:
            if (type == any_pointer::get_tag<int64_t>()) {
                state.bytecode.push_back(bytecode::unary_minus_int);
            }
            else if (type == any_pointer::get_tag<double>()) {
                state.bytecode.push_back(bytecode::unary_minus_double);
            }
            else error();
            break;
        case syntax::UnaryExpression::Not:
            if (type != any_pointer::get_tag<bool>()) error();
            state.bytecode.push_back(bytecode::not_bool);
            break;
    }
    return type;
}

}
