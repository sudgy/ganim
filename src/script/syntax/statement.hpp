#ifndef GANIM_SCRIPT_SYNTAX_STATEMENT_HPP
#define GANIM_SCRIPT_SYNTAX_STATEMENT_HPP

#include <optional>

#include "expression.hpp"
#include "type.hpp"

namespace ganim::syntax {

    class Statement;
    struct ExprStatement {
        Expression expression;
    };
    struct VarStatement {
        IdentifierExpression variable;
        Expression value;
        std::optional<Type> type;
        bool constant = false;
    };
    struct SetStatement {
        Expression lhs;
        Expression value;
    };
    struct IfStatement {
        Expression condition;
        std::vector<Statement> true_statements;
        std::vector<Statement> false_statements;
    };
    struct Statement {
        std::variant<
            ExprStatement,
            VarStatement,
            SetStatement,
            IfStatement
        > value;
    };
}

#endif
