#ifndef GANIM_SCRIPT_UNARY_EXPRESSION_HPP
#define GANIM_SCRIPT_UNARY_EXPRESSION_HPP

#include "expression.hpp"

#include <any>

namespace ganim::expressions {
    class UnaryPlus : public Expression {
        public:
            static std::unique_ptr<Expression> from_ast(
                Script& script,
                const syntax::Factor& ast
            );
            UnaryPlus(std::unique_ptr<Expression> base_value);

            virtual any_pointer value() override;
            virtual TypeID type() const override;
            virtual int line_number() const override
            {
                return M_line_number;
            }
            virtual int column_number() const override
            {
                return M_column_number;
            }

        private:
            std::any M_value;
            std::unique_ptr<Expression> M_base_value;
            int M_line_number;
            int M_column_number;
    };
    class UnaryMinus : public Expression {
        public:
            static std::unique_ptr<Expression> from_ast(
                Script& script,
                const syntax::Factor& ast
            );
            UnaryMinus(std::unique_ptr<Expression> base_value);

            virtual any_pointer value() override;
            virtual TypeID type() const override;
            virtual int line_number() const override
            {
                return M_line_number;
            }
            virtual int column_number() const override
            {
                return M_column_number;
            }

        private:
            std::any M_value;
            std::unique_ptr<Expression> M_base_value;
            int M_line_number;
            int M_column_number;
    };
}

#endif
