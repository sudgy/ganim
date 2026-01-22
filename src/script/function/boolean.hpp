#ifndef GANIM_SCRIPT_FUNCTION_BOOLEAN_HPP
#define GANIM_SCRIPT_FUNCTION_BOOLEAN_HPP

#include "function.hpp"

#include "script/owning_value.hpp"

namespace ganim::functions {
#define DECL(name, op)                                                         \
    class name : public Function {                                             \
        inline static Type TType = Type{any_pointer::get_tag<bool>()};         \
        public:                                                                \
            name() : Function(FunctionType(TType, {TType, TType})) {}          \
                                                                               \
            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)     \
                const override                                                 \
            {                                                                  \
                return std::make_unique<OwningValue<bool>>(                    \
                    *vs[0]->value().get_as<bool>() op                          \
                    *vs[1]->value().get_as<bool>());                           \
            }                                                                  \
    };
    DECL(And, and)
    DECL(Or, or)
    DECL(Xor, ^)
#undef DECL
#define DECL(name, op)                                                         \
    class name : public Function {                                             \
        inline static Type TType = Type{any_pointer::get_tag<bool>()};         \
        public:                                                                \
            name() : Function(FunctionType(TType, {TType, TType})) {}          \
                                                                               \
            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)     \
                const override                                                 \
            {                                                                  \
                return std::make_unique<OwningValue<bool>>(                    \
                    !(*vs[0]->value().get_as<bool>() op                        \
                      *vs[1]->value().get_as<bool>()));                        \
            }                                                                  \
    };
    DECL(Nand, and)
    DECL(Nor, or)
#undef DECL
    class Not : public Function {
        inline static Type TType = Type{any_pointer::get_tag<bool>()};
        public:
            Not() : Function(FunctionType(TType, {TType})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override
            {
                return std::make_unique<OwningValue<bool>>(
                    !(*vs[0]->value().get_as<bool>()));
            }
    };
}

#endif
