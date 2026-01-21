#ifndef GANIM_SCRIPT_FUNCTION_UNARY_HPP
#define GANIM_SCRIPT_FUNCTION_UNARY_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
    template <typename T>
    class UnaryPlus : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            UnaryPlus() : Function(FunctionType(TType, {TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    +(*vs[0]->value().get_as<T>()));
            }
    };

    template <typename T>
    class UnaryMinus : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            UnaryMinus() : Function(FunctionType(TType, {TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    -(*vs[0]->value().get_as<T>()));
            }
    };
}

#endif
