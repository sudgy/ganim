#include "binary.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/script_exception.hpp"

namespace ganim {

using namespace bytecode;

Type compile_binary_expression(
    CompilerState& state,
    const syntax::BinaryExpression& ast
)
{
    auto lhs = compile_expression(state, *ast.lhs);
    auto rhs = compile_expression(state, *ast.rhs);
    auto line_number = ast.lhs->line_number;
    auto column_number = ast.lhs->column_number;
    if (lhs != rhs) {
        throw CompileError(line_number, column_number,
            "Unable to do this operation on different types");
    }
    enum T {
        Int,
        Double
    };
    auto type = T();
    if (lhs == any_pointer::get_tag<int64_t>()) type = Int;
    else if (lhs == any_pointer::get_tag<double>()) type = Double;
    else {
        throw CompileError(line_number, column_number,
            "Unable to find this operation for this type");
    }
    switch (ast.op) {
    case syntax::BinaryExpression::Plus:
        switch (type) {
            case Int: state.bytecode.push_back(plus_int); break;
            case Double: state.bytecode.push_back(plus_double); break;
        }
        break;
    case syntax::BinaryExpression::Minus:
        switch (type) {
            case Int: state.bytecode.push_back(minus_int); break;
            case Double: state.bytecode.push_back(minus_double); break;
        }
        break;
    case syntax::BinaryExpression::Times:
        switch (type) {
            case Int: state.bytecode.push_back(mult_int); break;
            case Double: state.bytecode.push_back(mult_double); break;
        }
        break;
    case syntax::BinaryExpression::Divide:
        switch (type) {
            case Int: state.bytecode.push_back(div_int); break;
            case Double: state.bytecode.push_back(div_double); break;
        }
        break;
    case syntax::BinaryExpression::Modulo:
        if (type == Double) {
            throw CompileError(line_number, column_number,
                "Unable to find this operation for this type");
        }
        state.bytecode.push_back(mod_int);
        break;
    default:
        throw CompileError(line_number, column_number,
            "Unable to find this operation for this type");
    }
    return lhs;
}

}
