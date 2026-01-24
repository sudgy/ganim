#include "set.hpp"

#include "script/symbol_table.hpp"
#include "script/script_exception.hpp"
#include "script/owning_value.hpp"

#include "script/expression/function.hpp"

using namespace ganim;
using namespace ganim::commands;

Set::Set(
    SymbolTable& table,
    const syntax::SetStatement& ast
)
{
    M_lhs = Expression::from_ast(table, ast.lhs);
    M_value = Expression::from_ast(table, ast.value);
    if (M_lhs->type() != M_value->type()) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to set an incorrect type");
    }
    if (!M_lhs->modifiable()) {
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Attempt to modify an unmodifiable value");
    }
    auto fs = [&] -> std::vector<Function*> {
        switch (ast.op) {
            case syntax::SetStatement::None:
                return {};
            case syntax::SetStatement::Plus:
                return table.get_functions("__plus_eq__");
            case syntax::SetStatement::Minus:
                return table.get_functions("__minus_eq__");
            case syntax::SetStatement::Times:
                return table.get_functions("__mult_eq__");
            case syntax::SetStatement::Divide:
                return table.get_functions("__div_eq__");
            default: return {};
        }
    }();
    if (ast.op != syntax::SetStatement::None) {
        if (fs.empty()) {
            throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
                "Unable to find assignment operation");
        }
        auto param_types = std::vector<Type>{M_lhs->type(), M_value->type()};
        for (auto& f : fs) {
            if (f->get_input_types() == param_types) {
                auto params = std::vector<std::unique_ptr<Expression>>();
                params.emplace_back(std::move(M_lhs));
                params.emplace_back(std::move(M_value));
                M_value = std::make_unique<expressions::Function>(
                    *f,
                    std::move(params),
                    ast.lhs.line_number,
                    ast.lhs.column_number
                );
                return;
            }
        }
        throw CompileError(ast.lhs.line_number, ast.lhs.column_number,
            "Invalid assignment operation");
    }
}

void Set::execute() const
{
    if (M_lhs) M_lhs->modify(*M_value); // Normal assignment
    else M_value->value(); // Arithmetic assignment
}
