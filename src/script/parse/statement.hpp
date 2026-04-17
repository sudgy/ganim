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
        enum Operation {None, Plus, Minus, Times, Divide} op;
    };
    struct IfStatement {
        Expression condition;
        std::vector<Statement> true_statements;
        std::vector<Statement> false_statements;
    };
    struct LoopStatement {
        std::vector<Statement> loop_statements;
    };
    struct WhileStatement {
        Expression condition;
        std::vector<Statement> loop_statements;
    };
    // Statements made up of a single word, currently just break and continue
    struct WordStatement {
        std::string_view statement;
        int line_number = -1;
        int column_number = -1;
    };
    struct FunctionParameter {
        std::string_view name;
        Type type;
    };
    struct FunctionStatement {
        std::string_view name;
        Type type;
        std::vector<FunctionParameter> parameters;
        std::vector<Statement> body;
    };
    struct Statement {
        std::variant<
            ExprStatement,
            VarStatement,
            SetStatement,
            IfStatement,
            LoopStatement,
            WhileStatement,
            WordStatement,
            FunctionStatement
        > value;
    };
}

#endif
