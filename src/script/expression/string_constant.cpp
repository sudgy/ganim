#include "string_constant.hpp"

using namespace ganim;
using namespace ganim::expressions;

StringConstant::StringConstant(std::string string)
    : M_string(std::move(string)) {}

std::string StringConstant::as_string() const
{
    return M_string;
}

ExpressionType StringConstant::type() const
{
    return ExpressionType::String;
}
