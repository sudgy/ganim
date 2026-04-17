#include "function.hpp"

#include "overloaded.hpp"

#include "expression.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/compile/compiler.hpp"
#include "script/script_exception.hpp"

namespace ganim {

Value compile_function_expression(
    Compiler& compiler,
    const syntax::FunctionExpression& ast
)
{
    auto name = get<syntax::IdentifierExpression>(ast.name->value).name;
    if (name == "test_output") {
        for (auto& param : ast.parameters) {
            auto p = compile_expression(compiler, *param);

            std::visit(overloaded{
                [&](TypeID id) {
                    if (id == Type::get_tag<int64_t>()) {
                        compiler.write_byte(bytecode::test_int);
                        compiler.write_pop(1);
                    }
                    else if (id == Type::get_tag<double>()) {
                        compiler.write_byte(bytecode::test_double);
                        compiler.write_pop(1);
                    }
                    else if (id == Type::get_tag<bool>()) {
                        compiler.write_byte(bytecode::test_byte);
                        compiler.write_pop(1);
                    }
                    else {
                        throw std::runtime_error("Unable to test output this type");
                    }
                },
                [&](const auto*) {
                    throw std::runtime_error("Unable to test output this type");
                }
            }, p.type.value);
        }
        return {void_type, Value::rvalue()};
    }
    else {
        auto function = compiler.get_function(std::string(name));
        if (!function) {
            throw CompileError(ast.line_number, ast.column_number, std::format(
                "Unknown function \"{}\"", name));
        }
        if (function->input_types.size() != ast.parameters.size()) {
            throw CompileError(ast.line_number, ast.column_number, std::format(
                "Incorrect number of arguments passed to function \"{}\"",
                name));
        }
        auto size = uint64_t(0);
        for (int i = 0; i < ssize(ast.parameters); ++i) {
            auto& param = ast.parameters[i];
            auto& input_type = function->input_types[i];
            auto p = compile_expression(compiler, *param);
            if (p.type != input_type) {
                throw CompileError(ast.line_number, ast.column_number,
                    std::format("Incorrect argument type"));
            }
            size += p.type.size8();
        }
        compiler.write_enter(size);
        compiler.write_call(function->label);
        compiler.write_leave(0);
        return {void_type, Value::rvalue()};
    }
}

}
