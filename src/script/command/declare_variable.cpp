#include "declare_variable.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"
#include "script/variable.hpp"

using namespace ganim;
using namespace ganim::commands;

DeclareVariable::DeclareVariable(Script& script)
{
    auto name_token = script.consume_token();
    if (!name_token.is_identifier) {
        throw CompileError(
            name_token.line_number, name_token.column_number,
            "Expected identifier");
    }
    auto equals = script.consume_token();
    if (equals.string != "=") {
        throw CompileError(
            equals.line_number, equals.column_number, "Expected '='");
    }
    auto expression = script.get_expression();
    auto variable = std::unique_ptr<Value>();
    if (false) {}
#define X(T) \
    else if (expression->type() == any_pointer::get_tag<T>()) { \
        auto var = std::make_unique<Variable<T>>(); \
        M_on_execute = [expr = std::move(expression), &v = *var] { \
            v.initialize(*expr->value().get_as<T>()); \
        }; \
        variable = std::move(var); \
    }
#include "script/x_macro_type.hpp"
#undef X
    script.add_variable(
        name_token.string,
        std::move(variable),
        name_token.line_number,
        name_token.column_number
    );
    // TODO: Implement this
    //auto maybe_colon = script.get_token();
    //if (maybe_colon.string == ":") {
    //    script.consume_token();
    //    // Get a type?
    //}
}

void DeclareVariable::execute() const
{
    M_on_execute();
}
