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
    auto error = [&]{
        throw CompileError(line_number, column_number,
            "Unable to find this operation for this type");
    };
    if (lhs != rhs) {
        throw CompileError(line_number, column_number,
            "Unable to do this operation on different types");
    }
    enum T {
        Int,
        Double,
        Bool
    };
    auto type = T();
    if (lhs == any_pointer::get_tag<int64_t>()) type = Int;
    else if (lhs == any_pointer::get_tag<double>()) type = Double;
    else if (lhs == any_pointer::get_tag<bool>()) type = Bool;
    else error();
    switch (ast.op) {
    case syntax::BinaryExpression::Plus:
        switch (type) {
            case Int: state.bytecode.push_back(plus_int); break;
            case Double: state.bytecode.push_back(plus_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Minus:
        switch (type) {
            case Int: state.bytecode.push_back(minus_int); break;
            case Double: state.bytecode.push_back(minus_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Times:
        switch (type) {
            case Int: state.bytecode.push_back(mult_int); break;
            case Double: state.bytecode.push_back(mult_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Divide:
        switch (type) {
            case Int: state.bytecode.push_back(div_int); break;
            case Double: state.bytecode.push_back(div_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Modulo:
        if (type == Double) error();
        state.bytecode.push_back(mod_int);
        break;
    case syntax::BinaryExpression::LT:
    case syntax::BinaryExpression::LE:
    case syntax::BinaryExpression::GT:
    case syntax::BinaryExpression::GE:
    case syntax::BinaryExpression::Equal:
    case syntax::BinaryExpression::NotEqual:
        switch (type) {
            case Int:
                state.bytecode.push_back(compare_int);
                break;
            case Double:
                state.bytecode.push_back(compare_double);
                break;
            case Bool:
                if (ast.op != syntax::BinaryExpression::Equal and
                    ast.op != syntax::BinaryExpression::NotEqual)
                {
                    error();
                }
                state.bytecode.push_back(compare_byte);
                break;
        }
        switch (ast.op) {
            case syntax::BinaryExpression::LT:
                state.bytecode.push_back(jump_lt);
                break;
            case syntax::BinaryExpression::LE:
                state.bytecode.push_back(jump_le);
                break;
            case syntax::BinaryExpression::GT:
                state.bytecode.push_back(jump_gt);
                break;
            case syntax::BinaryExpression::GE:
                state.bytecode.push_back(jump_ge);
                break;
            case syntax::BinaryExpression::Equal:
                state.bytecode.push_back(jump_eq);
                break;
            case syntax::BinaryExpression::NotEqual:
                state.bytecode.push_back(jump_neq);
                break;
            default: break; // Provably impossible
        }
        state.bytecode.push_back(byte(4));
        state.bytecode.push_back(push_byte);
        state.bytecode.push_back(byte(0));
        state.bytecode.push_back(jump_short);
        state.bytecode.push_back(byte(2));
        state.bytecode.push_back(push_byte);
        state.bytecode.push_back(byte(1));
        return any_pointer::get_tag<bool>();
    case syntax::BinaryExpression::And:
    case syntax::BinaryExpression::Or:
    case syntax::BinaryExpression::Xor:
    case syntax::BinaryExpression::Nand:
    case syntax::BinaryExpression::Nor:
        if (type != Bool) error();
        switch (ast.op) {
            case syntax::BinaryExpression::And:
                state.bytecode.push_back(and_byte);
                break;
            case syntax::BinaryExpression::Or:
                state.bytecode.push_back(or_byte);
                break;
            case syntax::BinaryExpression::Xor:
                state.bytecode.push_back(xor_byte);
                break;
            case syntax::BinaryExpression::Nand:
                state.bytecode.push_back(and_byte);
                state.bytecode.push_back(not_bool);
                break;
            case syntax::BinaryExpression::Nor:
                state.bytecode.push_back(or_byte);
                state.bytecode.push_back(not_bool);
            default: break; // Provably impossible
        }
        break;
    default:
        error();
    }
    return lhs;
}

}
