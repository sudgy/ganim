#ifndef GANIM_SCRIPT_EXPRESSION_CONSTANT_HPP
#define GANIM_SCRIPT_EXPRESSION_CONSTANT_HPP

#include <utility>

#include "expression.hpp"

namespace ganim::expressions {
    template <typename T>
    class Constant : public Expression {
        public:
            Constant(
                T value,
                int line_number,
                int column_number
            )
            :   M_value(std::move(value)),
                M_line_number(line_number),
                M_column_number(column_number) {}
            virtual any_pointer value() override
            {
                return &M_value;
            }
            virtual TypeID type() const override
            {
                return any_pointer::get_tag<T>();
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
            T M_value;
            int M_line_number;
            int M_column_number;
    };
}

#endif
