#include "variable.hpp"

#include "overloaded.hpp"

#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/script_exception.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

Value compile_variable_expression(
    Compiler& compiler,
    const syntax::IdentifierExpression& ast
)
{
    auto variable = compiler.get_variable(std::string(ast.name));
    if (!variable) {
        throw CompileError(ast.line_number, ast.column_number, std::format(
            "Unknown identifier \"{}\"", ast.name));
    }
    auto size = variable->type.size8();
    for (uint64_t i = 0; i < size; ++i) {
        compiler.write_byte(bytecode::push_uint);
        compiler.write_byte(bytecode::param_stack_frame);
        compiler.write_parameter(variable->stack_frame_offset + i);
    }
    return {
        variable->type,
        Value::stack_frame(variable->stack_frame_offset),
        variable->modifiable
    };
}

}
