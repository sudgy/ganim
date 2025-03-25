#ifndef GANIM_SCRIPT_EXPRESSION_STRING_CONSTANT_HPP
#define GANIM_SCRIPT_EXPRESSION_STRING_CONSTANT_HPP

#include "expression.hpp"

namespace ganim::expressions {
    class StringConstant : public Expression {
        public:
            StringConstant(std::string string);
            virtual std::string as_string() const override;
            virtual ExpressionType type() const override;

        private:
            std::string M_string;
    };
}

#endif
