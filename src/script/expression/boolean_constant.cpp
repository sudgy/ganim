#include "boolean_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

BooleanConstant::BooleanConstant(
    bool value,
    int line_number,
    int column_number
) : M_value(value),
    M_line_number(line_number),
    M_column_number(column_number) {}

std::string BooleanConstant::as_string() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("Boolean constant {} is not a string.", M_value)
    );
}

std::int64_t BooleanConstant::as_integer() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("Boolean constant {} is not an integer.", M_value)
    );
}

bool BooleanConstant::as_boolean() const
{
    return M_value;
}

TypeID BooleanConstant::type() const
{
    return TypeID::Boolean;
}
