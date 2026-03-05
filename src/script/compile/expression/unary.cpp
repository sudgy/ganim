#include "unary.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/script_exception.hpp"

namespace ganim {

Value compile_unary_expression(
    CompilerState& state,
    const syntax::UnaryExpression& ast
)
{
    auto value = compile_expression(state, *ast.subexpression);
    auto error = [&]{
        throw CompileError(ast.line_number, ast.column_number,
            "Unable to find this operation for this type");
    };
    switch (ast.op) {
        case syntax::UnaryExpression::Plus:
            if (value.type != any_pointer::get_tag<int64_t>() and
                value.type != any_pointer::get_tag<double>())
            {
                error();
            }
            break;
        case syntax::UnaryExpression::Minus:
            if (value.type == any_pointer::get_tag<int64_t>()) {
                state.bytecode.push_back(bytecode::unary_minus_int);
            }
            else if (value.type == any_pointer::get_tag<double>()) {
                state.bytecode.push_back(bytecode::unary_minus_double);
            }
            else error();
            break;
        case syntax::UnaryExpression::Not:
            if (value.type != any_pointer::get_tag<bool>()) error();
            state.bytecode.push_back(bytecode::not_bool);
            break;
    }
    return value;
}

}
