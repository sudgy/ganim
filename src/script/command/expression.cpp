#include "expression.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

commands::Expression::Expression(std::unique_ptr<ganim::Expression> expr)
:   M_expr(std::move(expr)) {}

void commands::Expression::execute() const
{
    M_expr->value();
}
