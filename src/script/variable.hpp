#ifndef GANIM_SCRIPT_VARIABLE_HPP
#define GANIM_SCRIPT_VARIABLE_HPP

#include <memory>

#include "script/value.hpp"

namespace ganim {
    template <typename T>
    class Variable : public Value {
        public:
            Variable()=default;
            void initialize(T value)
            {
                M_value = std::make_unique<T>(std::move(value));
            }

            virtual any_pointer value() override
            {
                return M_value.get();
            }
            virtual TypeID type() const override
            {
                return any_pointer::get_tag<T>();
            }

        private:
            std::unique_ptr<T> M_value;
    };
}

#endif
