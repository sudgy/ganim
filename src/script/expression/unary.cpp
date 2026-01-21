#include "unary.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

UnaryPlus::UnaryPlus(
    std::unique_ptr<Expression> base_value
)
:   M_base_value(std::move(base_value)),
    M_line_number(M_base_value->line_number()),
    M_column_number(M_base_value->column_number())
{
    if (M_base_value->type() != Type{any_pointer::get_tag<std::int64_t>()} and
        M_base_value->type() != Type{any_pointer::get_tag<double>()})
    {
        throw CompileError(M_line_number, M_column_number,
            "Unary plus is not supported for this type");
    }
}

any_pointer UnaryPlus::value()
{
    auto val = M_base_value->value();
    if (M_base_value->type() == Type{any_pointer::get_tag<std::int64_t>()}) {
        M_value = *val.get_as<std::int64_t>();
        return any_cast<std::int64_t>(&M_value);
    }
    else if (M_base_value->type() == Type{any_pointer::get_tag<double>()}) {
        M_value = *val.get_as<double>();
        return any_cast<double>(&M_value);
    }
    else {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to perform unary plus on this type, although "
            "this should have been caught at compile time.  What happened?");
    }
}

Type UnaryPlus::type() const
{
    return M_base_value->type();
}

std::unique_ptr<Expression> UnaryPlus::from_ast(
    Script& script,
    const syntax::Factor& ast
)
{
    return std::make_unique<UnaryPlus>(Expression::from_ast(script, ast));
}

UnaryMinus::UnaryMinus(
    std::unique_ptr<Expression> base_value
)
:   M_base_value(std::move(base_value)),
    M_line_number(M_base_value->line_number()),
    M_column_number(M_base_value->column_number())
{
    if (M_base_value->type() != Type{any_pointer::get_tag<std::int64_t>()} and
        M_base_value->type() != Type{any_pointer::get_tag<double>()})
    {
        throw CompileError(M_line_number, M_column_number,
            "Unary minus is not supported for this type");
    }
}

any_pointer UnaryMinus::value()
{
    auto val = M_base_value->value();
    if (M_base_value->type() == Type{any_pointer::get_tag<std::int64_t>()}) {
        M_value = -*val.get_as<std::int64_t>();
        return any_cast<std::int64_t>(&M_value);
    }
    else if (M_base_value->type() == Type{any_pointer::get_tag<double>()}) {
        M_value = -*val.get_as<double>();
        return any_cast<double>(&M_value);
    }
    else {
        throw CompileError(M_line_number, M_column_number,
            "I don't know how to perform unary minus on this type, although "
            "this should have been caught at compile time.  What happened?");
    }
}

Type UnaryMinus::type() const
{
    return M_base_value->type();
}

std::unique_ptr<Expression> UnaryMinus::from_ast(
    Script& script,
    const syntax::Factor& ast
)
{
    return std::make_unique<UnaryMinus>(Expression::from_ast(script, ast));
}
