#include "compile.hpp"

#include "statement/statement.hpp"

namespace ganim {

std::vector<byte> compile(const std::vector<syntax::Statement>& ast)
{
    auto state = CompilerState();
    // I picked 8 randomly, maybe come back to this number later when you have
    // more data
    state.bytecode.reserve(ast.size() * 8);
    for (auto& statement : ast) {
        compile_statement(state, statement);
    }
    return std::move(state.bytecode);
}

}
