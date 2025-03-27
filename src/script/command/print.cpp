#include "print.hpp"

#include <iostream>

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

Print::Print(Script& script)
{
    auto& token = script.get_token();
    M_expr = script.get_expression();
    if (M_expr->type() != TypeID::String
            and M_expr->type() != TypeID::Integer) {
        throw CompileError(
            token.line_number, token.column_number,
            "Unable to convert expression to string");
    }
}

void Print::execute() const
{
    switch (M_expr->type()) {
    case TypeID::String:
        std::cout << M_expr->as_string() << "\n";
        break;
    case TypeID::Integer:
        std::cout << std::to_string(M_expr->as_integer()) << "\n";
        break;
    }
}
