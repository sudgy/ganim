#ifndef GANIM_SCRIPT_FUNCTION_PRINT_HPP
#define GANIM_SCRIPT_FUNCTION_PRINT_HPP

#include "function.hpp"

#include <functional>

namespace ganim::functions {

    void set_print_function(std::function<void(std::string_view)>);

    class PrintInt : public Function {
        public:
            PrintInt() : Function(FunctionType(
                nullptr,
                {Type{any_pointer::get_tag<std::int64_t>()}})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override;
    };
    class PrintDouble : public Function {
        public:
            PrintDouble() : Function(FunctionType(
                nullptr,
                {Type{any_pointer::get_tag<double>()}})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override;
    };
    class PrintString : public Function {
        public:
            PrintString() : Function(FunctionType(
                nullptr,
                {Type{any_pointer::get_tag<std::string>()}})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override;
    };
    class PrintBool : public Function {
        public:
            PrintBool() : Function(FunctionType(
                nullptr,
                {Type{any_pointer::get_tag<bool>()}})) {}

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs)
                const override;
    };
}

#endif
