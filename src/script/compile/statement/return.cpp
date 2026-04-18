#include "return.hpp"

#include <algorithm>

#include "script/compile/statement/statement.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/compile/compiler.hpp"
#include "script/compile/expression/expression.hpp"
#include "script/script_exception.hpp"

void ganim::compile_return_statement(
    Compiler& compiler,
    const syntax::ReturnStatement& ast
)
{
    const auto& function_type = compiler.get_current_function_type();
    if (!function_type) {
        throw CompileError(
            ast.line_number, ast.column_number,
            "return outside of a function"
        );
    }
    if (ast.value) {
        auto value = compile_expression(compiler, *ast.value);
        if (value.type != *function_type) {
            throw CompileError(
                ast.line_number, ast.column_number,
                "Return type mismatch"
            );
        }
        if (value.type == void_type) {
            compiler.write_pop(compiler.get_function_pop_size());
            compiler.write_byte(bytecode::ret);
        }
        else {
            auto size = value.type.size8();
            auto frame_size = compiler.get_function_pop_size();
            for (auto i = 0UZ; i < size; ++i) {
                compiler.write_byte(bytecode::move_stack2);
                compiler.write_parameter(i);
                compiler.write_parameter(frame_size + i);
            }
            compiler.write_pop(frame_size);
            compiler.write_byte(bytecode::ret);
        }
    }
    else {
        if (*function_type != void_type) {
            throw CompileError(
                ast.line_number, ast.column_number,
                "No return value given in a function returing a value"
            );
        }
        compiler.write_pop(compiler.get_function_pop_size());
        compiler.write_byte(bytecode::ret);
    }
}
