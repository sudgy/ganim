#include "constant.hpp"

#include "overloaded.hpp"

#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"

namespace ganim {

Value compile_constant_expression(
    CompilerState& state,
    const syntax::ConstantExpression& ast
)
{
    return std::visit(overloaded{
        [&](int64_t value) -> Value {
            state.bytecode.push_back(bytecode::push_int);
            state.write_parameter(value);
            return {any_pointer::get_tag<int64_t>(), Value::rvalue()};
        },
        [&](double value) -> Value {
            state.bytecode.push_back(bytecode::push_double);
            state.write_parameter(value);
            return {any_pointer::get_tag<double>(), Value::rvalue()};
        },
        [&](bool value) -> Value {
            state.bytecode.push_back(bytecode::push_byte);
            state.write_parameter(byte(value));
            return {any_pointer::get_tag<bool>(), Value::rvalue()};
        },
        [&](const std::string&) -> Value {
            throw std::runtime_error("Unable to push string (for now)");
        },
    }, ast.value);
}

}
