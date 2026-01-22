#ifndef GANIM_SCRIPT_FUNCTION_UNARY_HPP
#define GANIM_SCRIPT_FUNCTION_UNARY_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
#define DECL(name, op)                                                         \
    template <typename T>                                                      \
    class name : public Function {                                             \
        inline static Type TType = Type{any_pointer::get_tag<T>()};            \
        public:                                                                \
            name() : Function(FunctionType(TType, {TType})) {}                 \
                                                                               \
            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)     \
                const override                                                 \
            {                                                                  \
                return std::make_unique<OwningValue<T>>(                       \
                    op(*vs[0]->value().get_as<T>()));                          \
            }                                                                  \
    };
DECL(UnaryPlus, +)
DECL(UnaryMinus, -)
#undef DECL
}

#endif
