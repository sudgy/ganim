#include "declare_variable.hpp"

#include <utility>

#include "script/symbol_table.hpp"
#include "script/expression/expression.hpp"
#include "script/script_exception.hpp"
#include "script/owning_value.hpp"

using namespace ganim;
using namespace ganim::commands;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

DeclareVariable::DeclareVariable(
    SymbolTable& table,
    const syntax::VarStatement& ast
)
{
    auto name = ast.variable;
    auto expression = Expression::from_ast(table, ast.value);
    if (ast.type) {
        if (table.get_type(*ast.type) != expression->type()) {
            throw CompileError(
                ast.type->name.line_number,
                ast.type->name.column_number,
                "Type specifier does not match expression"
            );
        }
    }

    auto value = std::visit(overloaded{
        [&](const TypeID& type) -> std::unique_ptr<Value>
        {
            if (type == any_pointer::get_tag<std::int64_t>()) {
                auto result = std::make_unique<OwningValue<std::int64_t>>();
                result->set_modifiable(!ast.constant);
                M_on_execute = [value = std::move(expression), &r = *result] {
                    r.initialize(*value->value().get_as<std::int64_t>());
                };
                return result;
            }
            if (type == any_pointer::get_tag<double>()) {
                auto result = std::make_unique<OwningValue<double>>();
                result->set_modifiable(!ast.constant);
                M_on_execute = [value = std::move(expression), &r = *result] {
                    r.initialize(*value->value().get_as<double>());
                };
                return result;
            }
            if (type == any_pointer::get_tag<bool>()) {
                auto result = std::make_unique<OwningValue<bool>>();
                result->set_modifiable(!ast.constant);
                M_on_execute = [value = std::move(expression), &r = *result] {
                    r.initialize(*value->value().get_as<bool>());
                };
                return result;
            }
            if (type == any_pointer::get_tag<std::string>()) {
                auto result = std::make_unique<OwningValue<std::string>>();
                result->set_modifiable(!ast.constant);
                M_on_execute = [value = std::move(expression), &r = *result] {
                    r.initialize(*value->value().get_as<std::string>());
                };
                return result;
            }
            std::unreachable();
        },
        [&](const FunctionType*) -> std::unique_ptr<Value>
        {
            // TODO: Implement these
            std::unreachable();
        },
        [&](const CustomType*) -> std::unique_ptr<Value>
        {
            // TODO: Implement these
            std::unreachable();
        }
    }, expression->type().value);
    table.add_variable(
        name.name,
        std::move(value),
        name.line_number,
        name.column_number
    );
}

Command::ExecuteResult DeclareVariable::execute() const
{
    M_on_execute();
    return Nothing;
}
