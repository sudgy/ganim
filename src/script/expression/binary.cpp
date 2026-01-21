#include "binary.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

Binary::Binary(
    std::unique_ptr<Expression> lhs,
    std::unique_ptr<Expression> rhs,
    Operation operation
)
:   M_lhs(std::move(lhs)),
    M_rhs(std::move(rhs)),
    M_line_number(M_lhs->line_number()),
    M_column_number(M_lhs->column_number()),
    M_operation(operation)
{
    if (M_lhs->type() != M_rhs->type()) {
        throw CompileError(M_line_number, M_column_number,
            "Unable to do this operation on different types");
    }
    bool valid = false;
    if (M_lhs->type() == Type{any_pointer::get_tag<std::int64_t>()}) valid = true;
    if (M_lhs->type() == Type{any_pointer::get_tag<double>()}) {
        if (M_operation != Modulo) {
            valid = true;
        }
    }
    if (M_lhs->type() == Type{any_pointer::get_tag<std::string>()}) {
        if (M_operation == Plus) {
            valid = true;
        }
    }
    if (!valid) {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to do this operation on this type");
    }
}

namespace {
    template <typename T>
    auto calc(T& lhs, T& rhs, Binary::Operation op)
    {
        switch (op) {
        case Binary::Plus:
            return lhs + rhs;
        case Binary::Minus:
            return lhs - rhs;
        case Binary::Times:
            return lhs * rhs;
        case Binary::Divide:
            return lhs / rhs;
        default: throw 0;
        }
    }
}

any_pointer Binary::value()
{
    auto val1 = M_lhs->value();
    auto val2 = M_rhs->value();
    if (M_lhs->type() == Type{any_pointer::get_tag<std::int64_t>()}) {
        if (M_operation == Modulo) {
            M_value = *val1.get_as<std::int64_t>()
                    % *val2.get_as<std::int64_t>();
        }
        else {
            M_value = calc(
                *val1.get_as<std::int64_t>(),
                *val2.get_as<std::int64_t>(),
                M_operation
            );
        }
        return any_cast<std::int64_t>(&M_value);
    }
    else if (M_lhs->type() == Type{any_pointer::get_tag<double>()}) {
        M_value = calc(
            *val1.get_as<double>(),
            *val2.get_as<double>(),
            M_operation
        );
        return any_cast<double>(&M_value);
    }
    else if (M_lhs->type() == Type{any_pointer::get_tag<std::string>()}) {
        if (M_operation == Plus) {
            M_value = *val1.get_as<std::string>() + *val2.get_as<std::string>();
            return any_cast<std::string>(&M_value);
        }
    }
    throw CompileError(M_line_number, M_column_number,
        "I don't know how to add this type, although this should have been "
        "caught at compile time.  What happened?");
}

Type Binary::type() const
{
    return M_lhs->type();
}


std::unique_ptr<Expression> Binary::from_ast(
    Script& script,
    const syntax::Expression& ast
)
{
    if (ast.subexpression) {
        auto lhs = from_ast(script, *ast.subexpression);
        auto rhs = Binary::from_ast(script, ast.term);
        return std::make_unique<Binary>(
            std::move(lhs),
            std::move(rhs),
            ast.op == syntax::Expression::Plus ? Plus :
            Minus
        );
    }
    return Binary::from_ast(script, ast.term);
}

std::unique_ptr<Expression> Binary::from_ast(
    Script& script,
    const syntax::Term& ast
)
{
    if (ast.subterm) {
        auto lhs = from_ast(script, *ast.subterm);
        auto rhs = Expression::from_ast(script, ast.factor);
        return std::make_unique<Binary>(
            std::move(lhs),
            std::move(rhs),
            ast.op == syntax::Term::Times ? Times :
            ast.op == syntax::Term::Divide ? Divide :
            Modulo
        );
    }
    return Expression::from_ast(script, ast.factor);
}
