#include "constant.hpp"

#include "overloaded.hpp"

#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"

namespace ganim {

Type compile_constant_expression(
    CompilerState& state,
    const syntax::ConstantExpression& ast
)
{
    return std::visit(overloaded{
        [&](std::int64_t value) -> Type {
            state.bytecode.push_back(bytecode::push_int);
            state.write_parameter(value);
            return Type(any_pointer::get_tag<std::int64_t>());
        },
        [&](double value) -> Type {
            state.bytecode.push_back(bytecode::push_double);
            state.write_parameter(value);
            return Type(any_pointer::get_tag<double>());
        },
        [&](bool value) -> Type {
            state.bytecode.push_back(bytecode::push_byte);
            state.write_parameter(std::byte(value));
            return Type(any_pointer::get_tag<bool>());
        },
        [&](const std::string&) -> Type {
            throw std::runtime_error("Unable to push string (for now)");
        },
    }, ast.value);
}

}
