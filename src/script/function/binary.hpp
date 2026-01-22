#ifndef GANIM_SCRIPT_FUNCTION_BINARY_HPP
#define GANIM_SCRIPT_FUNCTION_BINARY_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
#define DECL(name, op)                                                         \
    template <typename T>                                                      \
    class name : public Function {                                             \
        inline static Type TType = Type{any_pointer::get_tag<T>()};            \
        public:                                                                \
            name() : Function(FunctionType(TType, {TType, TType})) {}          \
                                                                               \
            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)     \
                const override                                                 \
            {                                                                  \
                return std::make_unique<OwningValue<T>>(                       \
                    *vs[0]->value().get_as<T>() op                             \
                    *vs[1]->value().get_as<T>());                              \
            }                                                                  \
    };
    DECL(Add, +)
    DECL(Subtract, -)
    DECL(Multiply, *)
    DECL(Divide, /)
    DECL(Modulo, %)
#undef DECL
}

#endif
