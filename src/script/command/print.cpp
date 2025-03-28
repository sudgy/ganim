#include "print.hpp"

#include <iostream>

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

std::function<void(std::string_view)> Print::S_print_function
= [](std::string_view string)
{
    std::cout << string << "\n";
};

void Print::set_print_function(std::function<void(std::string_view)> func)
{
    S_print_function = std::move(func);
}

Print::Print(Script& script)
{
    // Uncomment everything in this function when get types that you don't know
    // how to print
    //auto& token = script.get_token();
    M_expr = script.get_expression();
    //if (M_expr->type() != TypeID::String
    //        and M_expr->type() != TypeID::Integer) {
    //    throw CompileError(
    //        token.line_number, token.column_number,
    //        "Unable to convert expression to string");
    //}
}

void Print::execute() const
{
    auto p = M_expr->value();
    if (auto value = p.get_as<std::string>()) {
        S_print_function(*value);
    }
    else if (auto value = p.get_as<std::int64_t>()) {
        S_print_function(std::format("{}", *value));
    }
    else if (auto value = p.get_as<bool>()) {
        S_print_function(*value ? "true" : "false");
    }
    else if (auto value = p.get_as<double>()) {
        S_print_function(std::format("{}", *value));
    }
    else {
        // This shouldn't ever happen but if you fail and forget to implement
        // this without adding a compile error it will
        throw RuntimeError(
            M_expr->line_number(), M_expr->column_number(),
            "Unknown type encountered when printing");
    }
}
