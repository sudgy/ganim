#ifndef GANIM_SCRIPT_FUNCTION_COMPARISON_HPP
#define GANIM_SCRIPT_FUNCTION_COMPARISON_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
    template <typename T>
    class LessThan : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            LessThan() : Function(FunctionType(
                Type{any_pointer::get_tag<bool>()},
                {TType, TType}
            )) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    *vs[0]->value().get_as<T>() <
                    *vs[1]->value().get_as<T>());
            }
    };
    template <typename T>
    class LessThanOrEqual : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            LessThanOrEqual() : Function(FunctionType(
                Type{any_pointer::get_tag<bool>()},
                {TType, TType}
            )) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    *vs[0]->value().get_as<T>() <=
                    *vs[1]->value().get_as<T>());
            }
    };
    template <typename T>
    class GreaterThan : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            GreaterThan() : Function(FunctionType(
                Type{any_pointer::get_tag<bool>()},
                {TType, TType}
            )) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    *vs[0]->value().get_as<T>() >
                    *vs[1]->value().get_as<T>());
            }
    };
    template <typename T>
    class GreaterThanOrEqual : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            GreaterThanOrEqual() : Function(FunctionType(
                Type{any_pointer::get_tag<bool>()},
                {TType, TType}
            )) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    *vs[0]->value().get_as<T>() >=
                    *vs[1]->value().get_as<T>());
            }
    };
}

#endif
