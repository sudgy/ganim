#ifndef GANIM_SCRIPT_FUNCTION_ASSIGNMENT_HPP
#define GANIM_SCRIPT_FUNCTION_ASSIGNMENT_HPP

#include "function.hpp"

#include "script/non_owning_value.hpp"

namespace ganim::functions {
#define DECL(name, op)                                                         \
    template <typename T>                                                      \
    class name : public Function {                                             \
        inline static Type TType = Type{any_pointer::get_tag<T>()};            \
        public:                                                                \
            name() : Function(FunctionType(void_type, {TType, TType})) {}      \
                                                                               \
            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)     \
                const override                                                 \
            {                                                                  \
                auto& v1 = *vs[0]->value().get_as<T>();                        \
                auto& v2 = *vs[1]->value().get_as<T>();                        \
                auto v = v1 op v2;                                             \
                auto nov = NonOwningValue<T>(v);                               \
                vs[0]->modify(nov);                                            \
                return nullptr;                                                \
            }                                                                  \
    };
    DECL(AddEq, +)
    DECL(SubtractEq, -)
    DECL(MultiplyEq, *)
    DECL(DivideEq, /)
#undef DECL
}

#endif
