#include "set.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"
#include "script/owning_value.hpp"

using namespace ganim;
using namespace ganim::commands;

Set::Set(
    Script& script,
    const syntax::SetStatement& ast
)
{
    M_lhs = Expression::from_ast(script, ast.lhs);
    M_value = Expression::from_ast(script, ast.value);
    if (M_lhs->type() != M_value->type()) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to set an incorrect type");
    }
    if (!M_lhs->modifiable()) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to modify an unmodifiable value");
    }
}

void Set::execute() const
{
    M_lhs->modify(*M_value);
}
