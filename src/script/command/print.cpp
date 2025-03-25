#include "print.hpp"

#include <iostream>

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

Print::Print(Script& script)
{
    auto token = script.consume_token();
    if (token.string[0] == '"') {
        M_string = token.string.substr(1, token.string.size() - 2);
    }
    else {
        throw ScriptException(
                token.line_number, token.column_number, "Expected string");
    }
}

void Print::execute() const
{
    std::cout << M_string << "\n";
}
