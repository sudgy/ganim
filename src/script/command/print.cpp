#include "print.hpp"

#include <iostream>

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

Print::Print(Script& script)
{
    auto& token = script.get_token();
    M_string = script.get_expression();
    if (M_string->type() != ExpressionType::String) {
        throw CompileError(
                token.line_number, token.column_number, "Expected string");
    }
}

void Print::execute() const
{
    std::cout << M_string->as_string() << "\n";
}
