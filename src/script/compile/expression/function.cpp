#include "expression.hpp"

#include "overloaded.hpp"

#include "function.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/any_pointer.hpp"

namespace ganim {

Type compile_function_expression(
    CompilerState& state,
    const syntax::FunctionExpression& ast
)
{
    auto name = get<syntax::IdentifierExpression>(ast.name->value).name;
    if (name == "test_output") {
        for (auto& param : ast.parameters) {
            auto p = compile_expression(state, *param);

            std::visit(overloaded{
                [&](TypeID id) {
                    if (id == any_pointer::get_tag<int64_t>()) {
                        state.bytecode.push_back(bytecode::test_int);
                        state.bytecode.push_back(bytecode::pop);
                        state.bytecode.push_back(byte(1));
                    }
                    else if (id == any_pointer::get_tag<double>()) {
                        state.bytecode.push_back(bytecode::test_double);
                        state.bytecode.push_back(bytecode::pop);
                        state.bytecode.push_back(byte(1));
                    }
                    else if (id == any_pointer::get_tag<bool>()) {
                        state.bytecode.push_back(bytecode::test_byte);
                        state.bytecode.push_back(bytecode::pop);
                        state.bytecode.push_back(byte(1));
                    }
                    else {
                        throw std::runtime_error("Unable to test output this type");
                    }
                },
                [&](const auto*) {
                    throw std::runtime_error("Unable to test output this type");
                }
            }, p.value);
        }
    }
    else throw std::runtime_error("Not yet implemented");
    return void_type;
}

}
