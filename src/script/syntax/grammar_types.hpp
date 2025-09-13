#ifndef GANIM_SCRIPT_SYNTAX_GRAMMAR_TYPES_HPP
#define GANIM_SCRIPT_SYNTAX_GRAMMAR_TYPES_HPP

#include <variant>
#include <cstdint>
#include <string>
#include <memory>

namespace ganim::syntax {
    struct Constant {
        std::variant<std::int64_t, double, bool, std::string_view> value = 0L;
        int line_number = -1;
        int column_number = -1;
    };
    struct Identifier {
        std::string_view name;
        int line_number = -1;
        int column_number = -1;
    };
    struct Expression;
    struct Factor {
        std::variant<Constant, Identifier, std::unique_ptr<Expression>> value;
        int line_number = -1;
        int column_number = -1;
    };
    struct Term {
        std::unique_ptr<Term> subterm;
        Factor factor;
        int line_number = -1;
        int column_number = -1;
    };
    struct Expression {
        std::unique_ptr<Expression> subexpression;
        Term term;
        int line_number = -1;
        int column_number = -1;
    };
    struct PrintStatement {
        Expression value;
    };
    struct VarStatement {
        Identifier variable;
        Expression value;
    };
    struct Statement {
        std::variant<PrintStatement, VarStatement> value;
    };
}

#endif
