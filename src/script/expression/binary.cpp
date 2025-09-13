#include "binary.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

Sum::Sum(
    std::unique_ptr<Expression> lhs,
    std::unique_ptr<Expression> rhs
)
:   M_lhs(std::move(lhs)),
    M_rhs(std::move(rhs)),
    M_line_number(M_lhs->line_number()),
    M_column_number(M_lhs->column_number())
{
    if (M_lhs->type() != M_rhs->type()) {
        throw CompileError(M_line_number, M_column_number,
            "Unable to add different types");
    }
    if (M_lhs->type() != any_pointer::get_tag<std::int64_t>() and
        M_lhs->type() != any_pointer::get_tag<double>() and
        M_lhs->type() != any_pointer::get_tag<std::string>())
    {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to add this type");
    }
}

any_pointer Sum::value()
{
    auto val1 = M_lhs->value();
    auto val2 = M_rhs->value();
    if (M_lhs->type() == any_pointer::get_tag<std::int64_t>()) {
        M_value = *val1.get_as<std::int64_t>() + *val2.get_as<std::int64_t>();
        return any_cast<std::int64_t>(&M_value);
    }
    else if (M_lhs->type() == any_pointer::get_tag<double>()) {
        M_value = *val1.get_as<double>() + *val2.get_as<double>();
        return any_cast<double>(&M_value);
    }
    else if (M_lhs->type() == any_pointer::get_tag<std::string>()) {
        M_value = *val1.get_as<std::string>() + *val2.get_as<std::string>();
        return any_cast<std::string>(&M_value);
    }
    else {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to add this type, although this should have been "
            "caught at compile time.  What happened?");
    }
}

TypeID Sum::type() const
{
    return M_lhs->type();
}

Product::Product(
    std::unique_ptr<Expression> lhs,
    std::unique_ptr<Expression> rhs
)
:   M_lhs(std::move(lhs)),
    M_rhs(std::move(rhs)),
    M_line_number(M_lhs->line_number()),
    M_column_number(M_lhs->column_number())
{
    if (M_lhs->type() != M_rhs->type()) {
        throw CompileError(M_line_number, M_column_number,
            "Unable to multiply different types");
    }
    if (M_lhs->type() != any_pointer::get_tag<std::int64_t>() and
        M_lhs->type() != any_pointer::get_tag<double>())
    {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to multiply this type");
    }
}

any_pointer Product::value()
{
    auto val1 = M_lhs->value();
    auto val2 = M_rhs->value();
    if (M_lhs->type() == any_pointer::get_tag<std::int64_t>()) {
        M_value = *val1.get_as<std::int64_t>() * *val2.get_as<std::int64_t>();
        return any_cast<std::int64_t>(&M_value);
    }
    else if (M_lhs->type() == any_pointer::get_tag<double>()) {
        M_value = *val1.get_as<double>() * *val2.get_as<double>();
        return any_cast<double>(&M_value);
    }
    else {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to multiply this type, although this should have "
            "been caught at compile time.  What happened?");
    }
}

TypeID Product::type() const
{
    return M_lhs->type();
}

std::unique_ptr<Expression> Sum::from_ast(
    Script& script,
    const syntax::Expression& ast
)
{
    if (ast.subexpression) {
        auto lhs = from_ast(script, *ast.subexpression);
        auto rhs = Product::from_ast(script, ast.term);
        return std::make_unique<Sum>(std::move(lhs), std::move(rhs));
    }
    return Product::from_ast(script, ast.term);
}

std::unique_ptr<Expression> Product::from_ast(
    Script& script,
    const syntax::Term& ast
)
{
    if (ast.subterm) {
        auto lhs = from_ast(script, *ast.subterm);
        auto rhs = Expression::from_ast(script, ast.factor);
        return std::make_unique<Product>(std::move(lhs), std::move(rhs));
    }
    return Expression::from_ast(script, ast.factor);
}
