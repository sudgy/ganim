#include "function.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

expressions::Function::Function(
    const ganim::Function& f,
    std::vector<std::unique_ptr<Expression>> inputs,
    int line_number,
    int column_number
)
:   M_f(&f),
    M_inputs(std::move(inputs)),
    M_line_number(line_number),
    M_column_number(column_number)
{
}

any_pointer expressions::Function::value()
{
    auto inputs = std::vector<Value*>();
    inputs.resize(M_inputs.size());
    for (int i = 0; i < ssize(inputs); ++i) {
        inputs[i] = M_inputs[i].get();
    }
    M_value = M_f->execute(inputs);
    if (M_value) return M_value->value();
    else return nullptr;
}

Type expressions::Function::type() const
{
    return M_f->get_result_type();
}

int expressions::Function::line_number() const
{
    return M_line_number;
}

int expressions::Function::column_number() const
{
    return M_column_number;
}

std::unique_ptr<Expression> expressions::Function::from_ast(
    Script& script,
    const syntax::Expression& ast
)
{
    if (ast.subexpression) {
        auto lhs = from_ast(script, *ast.subexpression);
        auto rhs = from_ast(script, ast.term);
        auto line_number = lhs->line_number();
        auto column_number = lhs->column_number();
        if (lhs->type() != rhs->type()) {
            throw CompileError(line_number, column_number,
                "Unable to do this operation on different types");
        }
        auto fs = [&] -> std::vector<ganim::Function*> {
            if (ast.op == syntax::Expression::Plus) {
                return script.get_functions("__plus__");
            }
            else { // Minus
                return script.get_functions("__minus__");
            }
        }();
        for (auto f : fs) {
            if (f->get_input_types()[0] == lhs->type() and
                f->get_input_types()[1] == rhs->type())
            {
                auto inputs = std::vector<std::unique_ptr<Expression>>();
                inputs.emplace_back(std::move(lhs));
                inputs.emplace_back(std::move(rhs));
                return std::make_unique<expressions::Function>(
                    *f,
                    std::move(inputs),
                    line_number,
                    column_number
                );
            }
        }
        throw CompileError(line_number, column_number,
            "Unable to find this operation for this type");
    }
    return Function::from_ast(script, ast.term);
}

std::unique_ptr<Expression> expressions::Function::from_ast(
    Script& script,
    const syntax::Term& ast
)
{
    if (ast.subterm) {
        auto lhs = from_ast(script, *ast.subterm);
        auto rhs = Expression::from_ast(script, ast.factor);
        auto line_number = lhs->line_number();
        auto column_number = lhs->column_number();
        if (lhs->type() != rhs->type()) {
            throw CompileError(line_number, column_number,
                "Unable to do this operation on different types");
        }
        auto fs = [&] -> std::vector<ganim::Function*> {
            if (ast.op == syntax::Term::Times) {
                return script.get_functions("__mult__");
            }
            else if (ast.op == syntax::Term::Divide) {
                return script.get_functions("__div__");
            }
            else { // Modulo
                return script.get_functions("__mod__");
            }
        }();
        for (auto f : fs) {
            if (f->get_input_types()[0] == lhs->type() and
                f->get_input_types()[1] == rhs->type())
            {
                auto inputs = std::vector<std::unique_ptr<Expression>>();
                inputs.emplace_back(std::move(lhs));
                inputs.emplace_back(std::move(rhs));
                return std::make_unique<expressions::Function>(
                    *f,
                    std::move(inputs),
                    line_number,
                    column_number
                );
            }
        }
        throw CompileError(line_number, column_number,
            "Unable to find this operation for this type");
    }
    return Expression::from_ast(script, ast.factor);
}

std::unique_ptr<Expression> expressions::Function::from_ast(
    Script& script,
    const syntax::Function& ast
)
{
    auto params = std::vector<std::unique_ptr<Expression>>();
    auto param_types = std::vector<Type>();
    for (auto& exp : ast.parameters) {
        params.push_back(Expression::from_ast(script, *exp));
        param_types.push_back(params.back()->type());
    }
    auto fs = script.get_functions(static_cast<std::string>(ast.name.name));
    for (auto& f : fs) {
        if (f->get_input_types() == param_types) {
            return std::make_unique<expressions::Function>(
                *f,
                std::move(params),
                ast.name.line_number,
                ast.name.column_number
            );
        }
    }
    throw CompileError(ast.name.line_number, ast.name.column_number,
        std::format("No matching function \"{}\"", ast.name.name));
}
