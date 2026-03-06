#include "unary.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/script_exception.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

Value compile_unary_expression(
    Compiler& compiler,
    const syntax::UnaryExpression& ast
)
{
    auto value = compile_expression(compiler, *ast.subexpression);
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
                compiler.write_byte(bytecode::unary_minus_int);
            }
            else if (value.type == any_pointer::get_tag<double>()) {
                compiler.write_byte(bytecode::unary_minus_double);
            }
            else error();
            break;
        case syntax::UnaryExpression::Not:
            if (value.type != any_pointer::get_tag<bool>()) error();
            compiler.write_byte(bytecode::not_bool);
            break;
    }
    return value;
}

}
