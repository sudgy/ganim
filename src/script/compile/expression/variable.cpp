#include "variable.hpp"

#include "overloaded.hpp"

#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/script_exception.hpp"

namespace ganim {

Type compile_variable_expression(
    CompilerState& state,
    const syntax::IdentifierExpression& ast
)
{
    auto variable = state.symbols.get_variable(std::string(ast.name));
    if (!variable) {
        throw CompileError(ast.line_number, ast.column_number, std::format(
            "Unknown identifier \"{}\"", ast.name));
    }
    auto size = variable->type.size8();
    for (uint64_t i = 0; i < size; ++i) {
        state.bytecode.push_back(bytecode::push_uint);
        state.bytecode.push_back(bytecode::param_stack_frame);
        state.write_parameter(variable->stack_frame_offset + i);
    }
    return variable->type;
}

}
