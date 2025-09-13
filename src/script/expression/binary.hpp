#ifndef GANIM_SCRIPT_BINARY_EXPRESSION_HPP
#define GANIM_SCRIPT_BINARY_EXPRESSION_HPP

#include <any>

#include "expression.hpp"

namespace ganim::expressions {
    class Binary : public Expression {
        public:
            static std::unique_ptr<Expression> from_ast(
                Script& script,
                const syntax::Expression& ast
            );
            static std::unique_ptr<Expression> from_ast(
                Script& script,
                const syntax::Term& ast
            );
            enum Operation {Plus, Minus, Times, Divide, Modulo};
            Binary(
                std::unique_ptr<Expression> lhs,
                std::unique_ptr<Expression> rhs,
                Operation operation
            );

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
            std::unique_ptr<Expression> M_lhs;
            std::unique_ptr<Expression> M_rhs;
            int M_line_number;
            int M_column_number;
            Operation M_operation;
    };
}

#endif
