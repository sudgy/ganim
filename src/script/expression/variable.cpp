#include "variable.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

std::unique_ptr<Variable> Variable::from_ast(
    Script& script,
    const syntax::IdentifierExpression& ast
)
{
    auto value = script.get_variable(std::string(ast.name));
    if (!value) {
        throw CompileError(ast.line_number, ast.column_number, std::format(
            "Unknown identifier \"{}\"", ast.name));
    }
    return std::make_unique<Variable>(
        *value,
        ast.line_number,
        ast.column_number
    );
}
