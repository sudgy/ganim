#include "symbol_table.hpp"

#include <ranges>

#include "script/script_exception.hpp"
#include "script/any_pointer.hpp"

using namespace ganim;

SymbolTable::SymbolTable()
{
    M_stack.push_back(this);
}

void SymbolTable::add_variable(
    std::string_view name,
    Type type,
    bool modifiable,
    int line_number,
    int column_number
)
{
    auto table = M_stack.back();
    auto name_string = std::string(name);
    if (table->M_variables.contains(name_string)) {
        throw CompileError(line_number, column_number, std::format(
                "A variable by the name \"{}\" already exists.", name));
    }
    table->M_variables[name_string] = {type, M_stack_frame_size, modifiable};
    auto size = type.size8();
    table->M_stack_frame_size += size;
}

std::optional<Variable> SymbolTable::get_variable(const std::string& name) const
{
    for (auto table : std::views::reverse(M_stack)) {
        auto it = table->M_variables.find(name);
        if (it != table->M_variables.end()) return it->second;
    }
    return std::nullopt;
}

Type SymbolTable::get_type(const syntax::Type& type) const
{
    // This function will change later
    if (type.name.name == "int") {
        return any_pointer::get_tag<std::int64_t>();
    }
    else if (type.name.name == "double") {
        return any_pointer::get_tag<double>();
    }
    else if (type.name.name == "string") {
        return any_pointer::get_tag<std::string>();
    }
    else if (type.name.name == "bool") {
        return any_pointer::get_tag<bool>();
    }
    throw CompileError(type.name.line_number, type.name.column_number,
        std::format("Unknown type \"{}\"", type.name.name));
}

void SymbolTable::push(SymbolTable& table)
{
    table.M_stack_frame_size = M_stack_frame_size;
    M_stack.push_back(&table);
}

uint64_t SymbolTable::pop()
{
    auto old_frame_size = M_stack.back()->M_stack_frame_size;
    M_stack.pop_back();
    auto new_frame_size = M_stack.back()->M_stack_frame_size;
    return old_frame_size - new_frame_size;
}
