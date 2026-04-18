#include "variable.hpp"

#include "script/bytecode/bytecodes.hpp"
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
        switch (variable->location) {
        case Variable::Global:
            compiler.write_byte(bytecode::param_global);
            compiler.write_parameter(variable->address + i);
            break;
        case Variable::StackFrame:
            compiler.write_byte(bytecode::param_stack_frame);
            compiler.write_parameter(variable->address + i);
            break;
        }
    }
    return {
        variable->type,
        variable->location == Variable::Global
            ? Value::Global
            : Value::StackFrame,
        variable->address,
        variable->modifiable
    };
}

}
