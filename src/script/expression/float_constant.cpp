#include "float_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

FloatConstant::FloatConstant(
    double value,
    int line_number,
    int column_number
) : M_value(value),
    M_line_number(line_number),
    M_column_number(column_number) {}

std::string FloatConstant::as_string() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("Float constant {} is not a string.", M_value)
    );
}

std::int64_t FloatConstant::as_integer() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("Float constant {} is not an integer.", M_value)
    );
}

bool FloatConstant::as_boolean() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("Float constant {} is not a boolean.", M_value)
    );
}

double FloatConstant::as_float() const
{
    return M_value;
}

TypeID FloatConstant::type() const
{
    return TypeID::Float;
}
