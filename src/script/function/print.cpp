#include "print.hpp"

#include <iostream>

namespace {
    std::function<void(std::string_view)> G_print_function
    = [](std::string_view string)
    {
        std::cout << string << "\n";
    };
}

using namespace ganim;
using namespace ganim::functions;

void functions::set_print_function(std::function<void(std::string_view)> func)
{
    G_print_function = std::move(func);
}

std::unique_ptr<Value> PrintInt::execute(std::vector<Value*> vs) const
{
    G_print_function(std::format("{}", *vs[0]->value().get_as<std::int64_t>()));
    return nullptr;
}

std::unique_ptr<Value> PrintDouble::execute(std::vector<Value*> vs) const
{
    G_print_function(std::format("{}", *vs[0]->value().get_as<double>()));
    return nullptr;
}

std::unique_ptr<Value> PrintString::execute(std::vector<Value*> vs) const
{
    G_print_function(*vs[0]->value().get_as<std::string>());
    return nullptr;
}

std::unique_ptr<Value> PrintBool::execute(std::vector<Value*> vs) const
{
    auto value = *vs[0]->value().get_as<bool>();
    G_print_function(value ? "true" : "false");
    return nullptr;
}
