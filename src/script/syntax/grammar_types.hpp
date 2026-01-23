#ifndef GANIM_SCRIPT_SYNTAX_GRAMMAR_TYPES_HPP
#define GANIM_SCRIPT_SYNTAX_GRAMMAR_TYPES_HPP

#include <variant>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace ganim::syntax {
    struct ConstantExpression {
        std::variant<std::int64_t, double, bool, std::string_view> value = 0L;
        int line_number = -1;
        int column_number = -1;
    };
    struct IdentifierExpression {
        std::string_view name;
        int line_number = -1;
        int column_number = -1;
    };
    struct Expression;
    struct FunctionExpression {
        std::unique_ptr<Expression> name;
        std::vector<std::unique_ptr<Expression>> parameters;
        int line_number = -1;
        int column_number = -1;

        FunctionExpression(
            std::unique_ptr<Expression> in_name,
            std::vector<std::unique_ptr<Expression>> in_parameters
        );
    };
    struct UnaryExpression {
        std::unique_ptr<Expression> subexpression;
        enum Operation {Plus, Minus, Not} op = Plus;
        int line_number = -1;
        int column_number = -1;

        UnaryExpression(
            std::unique_ptr<Expression> in_subexpression,
            Operation in_op
        );
    };
    struct BinaryExpression {
        std::unique_ptr<Expression> lhs;
        std::unique_ptr<Expression> rhs;
        enum Operation {
            Plus, Minus, Times, Divide, Modulo,
            LT, LE, GT, GE, Equal, NotEqual,
            And, Or, Xor, Nand, Nor
        } op = Plus;
        int line_number = -1;
        int column_number = -1;

        BinaryExpression(
            std::unique_ptr<Expression> in_lhs,
            std::unique_ptr<Expression> in_rhs,
            Operation in_op
        );
    };
    struct Expression {
        std::variant<
            ConstantExpression,
            IdentifierExpression,
            FunctionExpression,
            UnaryExpression,
            BinaryExpression
        > value;
        int line_number = -1;
        int column_number = -1;
    };
    struct ExprStatement {
        Expression expression;
    };
    struct VarStatement {
        IdentifierExpression variable;
        Expression value;
        bool constant = false;
    };
    struct SetStatement {
        Expression lhs;
        Expression value;
    };
    struct Statement {
        std::variant<ExprStatement, VarStatement, SetStatement> value;
    };
}

#endif
