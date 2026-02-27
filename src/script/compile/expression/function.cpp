#include "expression.hpp"

#include "overloaded.hpp"

#include "function.hpp"
#include "script/bytecode/bytecodes.hpp"

namespace ganim {

void compile_function_expression(
    CompilerState& state,
    const syntax::FunctionExpression& ast
)
{
    auto name = get<syntax::IdentifierExpression>(ast.name->value).name;
    if (name == "test_output") {
        for (auto& param : ast.parameters) {
            auto constant = get<syntax::ConstantExpression>(param->value);
            std::visit(overloaded{
                [&](std::int64_t value) {
                    state.bytecode.push_back(bytecode::test_int);
                    state.write_parameter(value);
                },
                [&](double value) {
                    state.bytecode.push_back(bytecode::test_double);
                    state.write_parameter(value);
                },
                [&](bool value) {
                    state.bytecode.push_back(bytecode::test_byte);
                    state.write_parameter(std::byte(value));
                },
                [&](const std::string&) {
                    throw std::runtime_error("Unable to test output string");
                },
            }, constant.value);
        }
    }
    else throw std::runtime_error("Not yet implemented");
}

}
