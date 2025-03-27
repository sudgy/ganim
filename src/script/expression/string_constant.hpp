#ifndef GANIM_SCRIPT_EXPRESSION_STRING_CONSTANT_HPP
#define GANIM_SCRIPT_EXPRESSION_STRING_CONSTANT_HPP

#include "expression.hpp"

namespace ganim::expressions {
    class StringConstant : public Expression {
        public:
            StringConstant(
                std::string string,
                int line_number,
                int column_number
            );
            virtual std::string as_string() const override;
            virtual std::int64_t as_integer() const override;
            virtual bool as_boolean() const override;
            virtual TypeID type() const override;

        private:
            std::string M_string;
            int M_line_number;
            int M_column_number;
    };
}

#endif
