#include "integer_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

IntegerConstant::IntegerConstant(
    std::int64_t value,
    int line_number,
    int column_number
) : M_value(value),
    M_line_number(line_number),
    M_column_number(column_number) {}

std::string IntegerConstant::as_string() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("Integer constant {} is not a string.", M_value)
    );
}

std::int64_t IntegerConstant::as_integer() const
{
    return M_value;
}

TypeID IntegerConstant::type() const
{
    return TypeID::Integer;
}
