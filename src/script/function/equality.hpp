#ifndef GANIM_SCRIPT_FUNCTION_EQUALITY_HPP
#define GANIM_SCRIPT_FUNCTION_EQUALITY_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
    template <typename T>
    class Equal : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            Equal() : Function(FunctionType(
                Type{any_pointer::get_tag<bool>()},
                {TType, TType}
            )) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    *vs[0]->value().get_as<T>() ==
                    *vs[1]->value().get_as<T>());
            }
    };

    template <typename T>
    class NotEqual : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            NotEqual() : Function(FunctionType(
                Type{any_pointer::get_tag<bool>()},
                {TType, TType}
            )) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    *vs[0]->value().get_as<T>() !=
                    *vs[1]->value().get_as<T>());
            }
    };
}

#endif
