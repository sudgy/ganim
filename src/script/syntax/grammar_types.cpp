#include "grammar_types.hpp"

using namespace ganim::syntax;

FunctionExpression::FunctionExpression(
    std::unique_ptr<Expression> in_name,
    std::vector<std::unique_ptr<Expression>> in_parameters
)
:   name(std::move(in_name)),
    parameters(std::move(in_parameters))
{
    line_number = name->line_number;
    column_number = name->column_number;
}

UnaryExpression::UnaryExpression(
    std::unique_ptr<Expression> in_subexpression,
    Operation in_op
)
:   subexpression(std::move(in_subexpression)),
    op(in_op)
{
    line_number = subexpression->line_number;
    column_number = subexpression->column_number;
}

BinaryExpression::BinaryExpression(
    std::unique_ptr<Expression> in_lhs,
    std::unique_ptr<Expression> in_rhs,
    Operation in_op
)
:   lhs(std::move(in_lhs)),
    rhs(std::move(in_rhs)),
    op(in_op)
{
    line_number = lhs->line_number;
    column_number = lhs->column_number;
}
