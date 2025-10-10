#include "declare_variable.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"
#include "script/owning_value.hpp"

using namespace ganim;
using namespace ganim::commands;

DeclareVariable::DeclareVariable(
    Script& script,
    const syntax::VarStatement& ast
)
{
    auto name = ast.variable;
    auto expression = Expression::from_ast(script, ast.value);
    auto delayed_value = make_delayed_owning_value_from(std::move(expression));
    M_on_execute = std::move(delayed_value.initialize);
    script.add_variable(
        name.name,
        std::move(delayed_value.value),
        name.line_number,
        name.column_number
    );
}

void DeclareVariable::execute() const
{
    M_on_execute();
}
