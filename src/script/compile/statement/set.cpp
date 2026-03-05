#include "set.hpp"

#include "overloaded.hpp"

#include "script/compile/expression/expression.hpp"
#include "script/script_exception.hpp"
#include "script/bytecode/bytecodes.hpp"

namespace ganim {

void compile_set_statement(
    CompilerState& state,
    const syntax::SetStatement& ast
)
{
    auto lhs = compile_expression(state, ast.lhs);
    if (!lhs.modifiable) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to modify a constant");
    }
    state.bytecode.push_back(bytecode::pop);
    state.write_parameter(lhs.type.size8());
    auto value = compile_expression(state, ast.value);
    if (lhs.type != value.type) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to set an incorrect type");
    }
    std::visit(overloaded{
        [&](Value::stack_frame offset) {
            auto size = lhs.type.size8();
            for (auto i = 0UZ; i < size; ++i) {
                state.bytecode.push_back(bytecode::move_stack);
                state.write_parameter(offset + size - i - 1);
            }
        },
        [&](Value::rvalue) {
            throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
                "Attempt to write to an rvalue");
        }
    }, lhs.location);
}

}
