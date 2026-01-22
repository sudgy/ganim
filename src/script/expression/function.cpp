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
    const syntax::FunctionExpression& ast
)
{
    auto params = std::vector<std::unique_ptr<Expression>>();
    auto param_types = std::vector<Type>();
    for (auto& exp : ast.parameters) {
        params.push_back(Expression::from_ast(script, *exp));
        param_types.push_back(params.back()->type());
    }
    // For now, function lookup will get more complicated later
    auto name = get<syntax::IdentifierExpression>(ast.name->value);
    auto fs = script.get_functions(static_cast<std::string>(name.name));
    for (auto& f : fs) {
        if (f->get_input_types() == param_types) {
            return std::make_unique<expressions::Function>(
                *f,
                std::move(params),
                name.line_number,
                name.column_number
            );
        }
    }
    throw CompileError(name.line_number, name.column_number,
        std::format("No matching function \"{}\"", name.name));
}

std::unique_ptr<Expression> expressions::Function::from_ast(
    Script& script,
    const syntax::BinaryExpression& ast
)
{
    auto lhs = Expression::from_ast(script, *ast.lhs);
    auto rhs = Expression::from_ast(script, *ast.rhs);
    auto line_number = lhs->line_number();
    auto column_number = lhs->column_number();
    if (lhs->type() != rhs->type()) {
        throw CompileError(line_number, column_number,
            "Unable to do this operation on different types");
    }
    auto fs = [&] -> std::vector<ganim::Function*> {
        switch (ast.op) {
        case syntax::BinaryExpression::Plus:
            return script.get_functions("__plus__");
        case syntax::BinaryExpression::Minus:
            return script.get_functions("__minus__");
        case syntax::BinaryExpression::Times:
            return script.get_functions("__mult__");
        case syntax::BinaryExpression::Divide:
            return script.get_functions("__div__");
        case syntax::BinaryExpression::Modulo:
            return script.get_functions("__mod__");
        case syntax::BinaryExpression::Equal:
            return script.get_functions("__eq__");
        case syntax::BinaryExpression::NotEqual:
            return script.get_functions("__neq__");
        case syntax::BinaryExpression::LT:
            return script.get_functions("__lt__");
        case syntax::BinaryExpression::LE:
            return script.get_functions("__le__");
        case syntax::BinaryExpression::GT:
            return script.get_functions("__gt__");
        case syntax::BinaryExpression::GE:
            return script.get_functions("__ge__");
        }
        return {};
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

std::unique_ptr<Expression> expressions::Function::from_ast(
    Script& script,
    const syntax::UnaryExpression& ast
)
{
    auto sub = Expression::from_ast(script, *ast.subexpression);
    auto line_number = sub->line_number();
    auto column_number = sub->column_number();
    auto fs = [&] -> std::vector<ganim::Function*> {
        if (ast.plus_sign) {
            return script.get_functions("__unary_plus__");
        }
        else {
            return script.get_functions("__unary_minus__");
        }
    }();
    for (auto f : fs) {
        if (f->get_input_types()[0] == sub->type()) {
            auto inputs = std::vector<std::unique_ptr<Expression>>();
            inputs.emplace_back(std::move(sub));
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
