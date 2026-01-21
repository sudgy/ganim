#ifndef GANIM_SCRIPT_EXPRESSION_FUNCTION_HPP
#define GANIM_SCRIPT_EXPRESSION_FUNCTION_HPP

#include "expression.hpp"
#include "script/function/function.hpp"

namespace ganim::expressions {
    class Function : public Expression {
        public:
            static std::unique_ptr<Expression> from_ast(
                Script& script,
                const syntax::Expression& ast
            );
            static std::unique_ptr<Expression> from_ast(
                Script& script,
                const syntax::Term& ast
            );
            Function(
                const ganim::Function& f,
                std::vector<std::unique_ptr<Expression>> inputs,
                int line_number,
                int column_number
            );

            virtual any_pointer value() override;
            virtual Type type() const override;
            virtual int line_number() const override;
            virtual int column_number() const override;

        private:
            const ganim::Function* M_f = nullptr;
            std::vector<std::unique_ptr<Expression>> M_inputs;
            int M_line_number = -1;
            int M_column_number = -1;
            std::unique_ptr<Value> M_value = nullptr;
    };
}

#endif
