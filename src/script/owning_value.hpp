#ifndef GANIM_SCRIPT_OWNING_VALUE_HPP
#define GANIM_SCRIPT_OWNING_VALUE_HPP

#include <memory>
#include <functional>

#include "script/value.hpp"

namespace ganim {
    template <typename T>
    class OwningValue : public Value {
        public:
            OwningValue()=default;
            explicit OwningValue(T value)
            : M_value(std::make_unique<T>(std::move(value))) {}
            void initialize(T value)
            {
                M_value = std::make_unique<T>(std::move(value));
            }

            virtual any_pointer value() override
            {
                return M_value.get();
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
            std::unique_ptr<T> M_value;
            bool M_modifiable = false;
    };
}

#endif
