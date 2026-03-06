#include "constant.hpp"

#include "overloaded.hpp"

#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"
#include "script/compile/compiler.hpp"

namespace ganim {

Value compile_constant_expression(
    Compiler& compiler,
    const syntax::ConstantExpression& ast
)
{
    return std::visit(overloaded{
        [&](int64_t value) -> Value {
            compiler.write_byte(bytecode::push_int);
            compiler.write_parameter(value);
            return {any_pointer::get_tag<int64_t>(), Value::rvalue()};
        },
        [&](double value) -> Value {
            compiler.write_byte(bytecode::push_double);
            compiler.write_parameter(value);
            return {any_pointer::get_tag<double>(), Value::rvalue()};
        },
        [&](bool value) -> Value {
            compiler.write_byte(bytecode::push_byte);
            compiler.write_parameter(byte(value));
            return {any_pointer::get_tag<bool>(), Value::rvalue()};
        },
        [&](const std::string&) -> Value {
            throw std::runtime_error("Unable to push string (for now)");
        },
    }, ast.value);
}

}
