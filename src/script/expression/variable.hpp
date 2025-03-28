#ifndef GANIM_SCRIPT_EXPRESSION_VARIABLE_HPP
#define GANIM_SCRIPT_EXPRESSION_VARIABLE_HPP

#include <utility>

#include "expression.hpp"

namespace ganim::expressions {
    class Variable : public Expression {
        public:
            Variable(
                Value& variable,
                int line_number,
                int column_number
            )
            :   M_variable(&variable),
                M_line_number(line_number),
                M_column_number(column_number) {}
            virtual any_pointer value() override
            {
                return M_variable->value();
            }
            virtual TypeID type() const override
            {
                return M_variable->type();
            }
            virtual int line_number() const override
            {
                return M_line_number;
            }
            virtual int column_number() const override
            {
                return M_column_number;
            }

        private:
            Value* M_variable;
            int M_line_number;
            int M_column_number;
    };
}

#endif
