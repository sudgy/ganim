#include "print.hpp"

#include <iostream>

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

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
        std::cout << *value << "\n";
    }
    else if (auto value = p.get_as<std::uint64_t>()) {
        std::cout << std::to_string(*value) << "\n";
    }
    else if (auto value = p.get_as<bool>()) {
        std::cout << (*value ? "true" : "false") << "\n";
    }
    else if (auto value = p.get_as<double>()) {
        std::cout << *value << "\n";
    }
    else {
        // This shouldn't ever happen but if you fail and forget to implement
        // this without adding a compile error it will
        throw RuntimeError(
            M_expr->line_number(), M_expr->column_number(),
            "Unknown type encountered when printing");
    }
}
