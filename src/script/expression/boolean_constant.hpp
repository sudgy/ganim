#ifndef GANIM_SCRIPT_EXPRESSION_BOOLEAN_CONSTANT_HPP
#define GANIM_SCRIPT_EXPRESSION_BOOLEAN_CONSTANT_HPP

#include "expression.hpp"

namespace ganim::expressions {
    class BooleanConstant : public Expression {
        public:
            BooleanConstant(
                bool value,
                int line_number,
                int column_number
            );
            virtual std::string as_string() const override;
            virtual std::int64_t as_integer() const override;
            virtual bool as_boolean() const override;
            virtual double as_float() const override;
            virtual TypeID type() const override;

        private:
            bool M_value;
            int M_line_number;
            int M_column_number;
    };
}

#endif
