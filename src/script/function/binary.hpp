#ifndef GANIM_SCRIPT_FUNCTION_BINARY_HPP
#define GANIM_SCRIPT_FUNCTION_BINARY_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
    template <typename T>
    class Add : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            Add() : Function(FunctionType(TType, {TType, TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    *vs[0]->value().get_as<T>() +
                    *vs[1]->value().get_as<T>());
            }
    };
}

namespace ganim::functions {
    template <typename T>
    class Subtract : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            Subtract() : Function(FunctionType(TType, {TType, TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    *vs[0]->value().get_as<T>() -
                    *vs[1]->value().get_as<T>());
            }
    };
}

namespace ganim::functions {
    template <typename T>
    class Multiply : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            Multiply() : Function(FunctionType(TType, {TType, TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    *vs[0]->value().get_as<T>() *
                    *vs[1]->value().get_as<T>());
            }
    };
}

namespace ganim::functions {
    template <typename T>
    class Divide : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            Divide() : Function(FunctionType(TType, {TType, TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    *vs[0]->value().get_as<T>() /
                    *vs[1]->value().get_as<T>());
            }
    };
}

namespace ganim::functions {
    template <typename T>
    class Modulo : public Function {
        inline static Type TType = Type{any_pointer::get_tag<T>()};
        public:
            Modulo() : Function(FunctionType(TType, {TType, TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<T>>(
                    *vs[0]->value().get_as<T>() %
                    *vs[1]->value().get_as<T>());
            }
    };
}

#endif
