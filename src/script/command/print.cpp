#include "print.hpp"

#include <iostream>

#include "script/script.hpp"

using namespace ganim;
using namespace ganim::commands;

Print::Print(Script& script)
{
    auto token = script.consume_token().string;
    if (token[0] == '"') {
        M_string = token.substr(1, token.size() - 2);
    }
    else {
        // Error
    }
}

void Print::execute() const
{
    std::cout << M_string << "\n";
}
