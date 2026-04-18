#include "constant.hpp"

#include "overloaded.hpp"

#include "script/bytecode/bytecodes.hpp"
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
            return {Type::get_tag<int64_t>(), Value::RValue};
        },
        [&](double value) -> Value {
            compiler.write_byte(bytecode::push_double);
            compiler.write_parameter(value);
            return {Type::get_tag<double>(), Value::RValue};
        },
        [&](bool value) -> Value {
            compiler.write_byte(bytecode::push_byte);
            compiler.write_parameter(byte(value));
            return {Type::get_tag<bool>(), Value::RValue};
        },
        [&](const std::string&) -> Value {
            throw std::runtime_error("Unable to push string (for now)");
        },
    }, ast.value);
}

}
