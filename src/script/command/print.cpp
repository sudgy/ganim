#include "print.hpp"

#include <iostream>

#include "script/script.hpp"

using namespace ganim;
using namespace ganim::commands;

Print::Print(Script& script)
{
    M_string = script.consume_token().string;
}

void Print::execute() const
{
    std::cout << M_string << "\n";
}
