#include "binary.hpp"

#include "expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/script_exception.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

using namespace bytecode;

Value compile_binary_expression(
    Compiler& compiler,
    const syntax::BinaryExpression& ast
)
{
    auto lhs = compile_expression(compiler, *ast.lhs);
    auto rhs = compile_expression(compiler, *ast.rhs);
    auto line_number = ast.lhs->line_number;
    auto column_number = ast.lhs->column_number;
    auto error = [&]{
        throw CompileError(line_number, column_number,
            "Unable to find this operation for this type");
    };
    if (lhs.type != rhs.type) {
        throw CompileError(line_number, column_number,
            "Unable to do this operation on different types");
    }
    enum T {
        Int,
        Double,
        Bool
    };
    auto type = T();
    if (lhs.type == Type::get_tag<int64_t>()) type = Int;
    else if (lhs.type == Type::get_tag<double>()) type = Double;
    else if (lhs.type == Type::get_tag<bool>()) type = Bool;
    else error();
    switch (ast.op) {
    case syntax::BinaryExpression::Plus:
        switch (type) {
            case Int: compiler.write_byte(plus_int); break;
            case Double: compiler.write_byte(plus_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Minus:
        switch (type) {
            case Int: compiler.write_byte(minus_int); break;
            case Double: compiler.write_byte(minus_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Times:
        switch (type) {
            case Int: compiler.write_byte(mult_int); break;
            case Double: compiler.write_byte(mult_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Divide:
        switch (type) {
            case Int: compiler.write_byte(div_int); break;
            case Double: compiler.write_byte(div_double); break;
            default: error();
        }
        break;
    case syntax::BinaryExpression::Modulo:
        if (type == Double) error();
        compiler.write_byte(mod_int);
        break;
    case syntax::BinaryExpression::LT:
    case syntax::BinaryExpression::LE:
    case syntax::BinaryExpression::GT:
    case syntax::BinaryExpression::GE:
    case syntax::BinaryExpression::Equal:
    case syntax::BinaryExpression::NotEqual:
        switch (type) {
            case Int:
                compiler.write_byte(compare_int);
                break;
            case Double:
                compiler.write_byte(compare_double);
                break;
            case Bool:
                if (ast.op != syntax::BinaryExpression::Equal and
                    ast.op != syntax::BinaryExpression::NotEqual)
                {
                    error();
                }
                compiler.write_byte(compare_byte);
                break;
        }
        switch (ast.op) {
            case syntax::BinaryExpression::LT:
                compiler.write_byte(jump_lt);
                break;
            case syntax::BinaryExpression::LE:
                compiler.write_byte(jump_le);
                break;
            case syntax::BinaryExpression::GT:
                compiler.write_byte(jump_gt);
                break;
            case syntax::BinaryExpression::GE:
                compiler.write_byte(jump_ge);
                break;
            case syntax::BinaryExpression::Equal:
                compiler.write_byte(jump_eq);
                break;
            case syntax::BinaryExpression::NotEqual:
                compiler.write_byte(jump_neq);
                break;
            default: break; // Provably impossible
        }
        compiler.write_byte(byte(4));
        compiler.write_byte(push_byte);
        compiler.write_byte(byte(0));
        compiler.write_byte(jump_short);
        compiler.write_byte(byte(2));
        compiler.write_byte(push_byte);
        compiler.write_byte(byte(1));
        return {Type::get_tag<bool>(), Value::RValue};
    case syntax::BinaryExpression::And:
    case syntax::BinaryExpression::Or:
    case syntax::BinaryExpression::Xor:
    case syntax::BinaryExpression::Nand:
    case syntax::BinaryExpression::Nor:
        if (type != Bool) error();
        switch (ast.op) {
            case syntax::BinaryExpression::And:
                compiler.write_byte(and_byte);
                break;
            case syntax::BinaryExpression::Or:
                compiler.write_byte(or_byte);
                break;
            case syntax::BinaryExpression::Xor:
                compiler.write_byte(xor_byte);
                break;
            case syntax::BinaryExpression::Nand:
                compiler.write_byte(and_byte);
                compiler.write_byte(not_bool);
                break;
            case syntax::BinaryExpression::Nor:
                compiler.write_byte(or_byte);
                compiler.write_byte(not_bool);
            default: break; // Provably impossible
        }
        break;
    default:
        error();
    }
    return lhs;
}

}
