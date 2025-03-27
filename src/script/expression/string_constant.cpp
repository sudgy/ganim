#include "string_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

StringConstant::StringConstant(
    std::string string,
    int line_number,
    int column_number
) : M_string(std::move(string)),
    M_line_number(line_number),
    M_column_number(column_number) {}

std::string StringConstant::as_string() const
{
    return M_string;
}

std::int64_t StringConstant::as_integer() const
{
    throw RuntimeError(
        M_line_number,
        M_column_number,
        std::format("String constant \"{}\" is not an integer.", M_string)
    );
}

ExpressionType StringConstant::type() const
{
    return ExpressionType::String;
}
