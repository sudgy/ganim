#ifndef GANIM_SCRIPT_NON_OWNING_VALUE_HPP
#define GANIM_SCRIPT_NON_OWNING_VALUE_HPP

#include <memory>
#include <functional>

#include "script/value.hpp"

namespace ganim {
    template <typename T>
    class NonOwningValue : public Value {
        public:
            NonOwningValue()=default;
            explicit NonOwningValue(T& value)
            : M_value(&value) {}

            virtual any_pointer value() override
            {
                return M_value;
            }
            virtual Type type() const override
            {
                // TODO: Figure out how to make this work with other types
                return {any_pointer::get_tag<T>()};
            }
            void set_modifiable(bool modifiable)
            {
                M_modifiable = modifiable;
            }
            virtual bool modifiable() const
            {
                return M_modifiable;
            }
            virtual void modify(Value& value)
            {
                *M_value = *value.value().get_as<T>();
            }

        private:
            T* M_value;
            bool M_modifiable = false;
    };
}

#endif
