#include "print.hpp"

#include <iostream>

using namespace ganim;
using namespace ganim::commands;

Print::Print(
    std::string_view full_command_string,
    std::string_view command_string
)
:   CommandFactoryBase<Print>(full_command_string)
{
    M_string = command_string;
}

void Print::execute() const
{
    std::cout << M_string << "\n";
}
