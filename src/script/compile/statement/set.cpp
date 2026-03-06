#include "set.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/script_exception.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

void compile_set_statement(
    Compiler& compiler,
    const syntax::SetStatement& ast
)
{
    auto lhs = compile_expression(compiler, ast.lhs);
    if (!lhs.modifiable) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to modify a constant");
    }
    if (ast.op == syntax::SetStatement::None) {
        compiler.write_pop(lhs.type.size8());
    }
    auto value = compile_expression(compiler, ast.value);
    if (lhs.type != value.type) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to set an incorrect type");
    }
    enum Type {Int, Double, Other} type = Other;
    if (lhs.type == any_pointer::get_tag<int64_t>()) type = Int;
    else if (lhs.type == any_pointer::get_tag<double>()) type = Double;
    auto error = [&]{
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Invalid assignment operation");
    };
    switch (ast.op) {
    case syntax::SetStatement::None:
        break;
    case syntax::SetStatement::Plus:
        switch (type) {
            case Int: compiler.write_byte(bytecode::plus_int); break;
            case Double: compiler.write_byte(bytecode::plus_double); break;
            default: error();
        }
        break;
    case syntax::SetStatement::Minus:
        switch (type) {
            case Int: compiler.write_byte(bytecode::minus_int); break;
            case Double: compiler.write_byte(bytecode::minus_double);break;
            default: error();
        }
        break;
    case syntax::SetStatement::Times:
        switch (type) {
            case Int: compiler.write_byte(bytecode::mult_int); break;
            case Double: compiler.write_byte(bytecode::mult_double); break;
            default: error();
        }
        break;
    case syntax::SetStatement::Divide:
        switch (type) {
            case Int: compiler.write_byte(bytecode::div_int); break;
            case Double: compiler.write_byte(bytecode::div_double); break;
            default: error();
        }
        break;
    }
    std::visit(overloaded{
        [&](Value::stack_frame offset) {
            auto size = lhs.type.size8();
            for (auto i = 0UZ; i < size; ++i) {
                compiler.write_byte(bytecode::move_stack);
                compiler.write_parameter(offset + size - i - 1);
            }
        },
        [&](Value::rvalue) {
            throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
                "Attempt to write to an rvalue");
        }
    }, lhs.location);
}

}
