#include "function.hpp"

#include <algorithm>

#include "script/compile/statement/statement.hpp"
#include "script/bytecode/bytecodes.hpp"
#include "script/compile/compiler.hpp"

void ganim::compile_function_statement(
    Compiler& compiler,
    const syntax::FunctionStatement& ast
)
{
    auto params = std::vector<Type>();
    std::ranges::transform(ast.parameters, std::back_inserter(params),
    [&](auto& p) {
        return compiler.get_type(p.type);
    });
    auto function_label = compiler.add_function(
        ast.name,
        params,
        compiler.get_type(ast.type)
    );
    auto end_of_function = compiler.get_next_label();

    compiler.write_jump(end_of_function);
    compiler.add_label_reference(function_label);
    compiler.push_symbols();
    for (auto& [name, type] : ast.parameters) {
        compiler.add_variable(name, compiler.get_type(type));
    }
    for (auto& statement : ast.body) {
        compile_statement(compiler, statement);
    }
    compiler.write_pop(compiler.pop_symbols());
    compiler.write_byte(bytecode::ret);
    compiler.add_label_reference(end_of_function);
}
