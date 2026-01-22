#ifndef GANIM_SCRIPT_FUNCTION_COMPARISON_HPP
#define GANIM_SCRIPT_FUNCTION_COMPARISON_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
#define DECL(name, op)                                                         \
    template <typename T>                                                      \
    class name : public Function {                                             \
        inline static Type TType = Type{any_pointer::get_tag<T>()};            \
        public:                                                                \
            name() : Function(FunctionType(                                    \
                Type{any_pointer::get_tag<bool>()},                            \
                {TType, TType}                                                 \
            )) {}                                                              \
                                                                               \
            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)     \
                const override                                                 \
            {                                                                  \
                return std::make_unique<OwningValue<bool>>(                    \
                    *vs[0]->value().get_as<T>() op                             \
                    *vs[1]->value().get_as<T>());                              \
            }                                                                  \
    };
DECL(LessThan, <)
DECL(LessThanOrEqual, <=)
DECL(GreaterThan, >)
DECL(GreaterThanOrEqual, >=)
#undef DECL
}

#endif
