#include "symbol_table.hpp"

#include "script/script_exception.hpp"
#include "script/any_pointer.hpp"

using namespace ganim;

void SymbolTable::add_variable(
    std::string_view name,
    Type type,
    bool modifiable,
    int line_number,
    int column_number
)
{
    auto name_string = std::string(name);
    if (M_variables.contains(name_string)) {
        throw CompileError(line_number, column_number, std::format(
                "A variable by the name \"{}\" already exists.", name));
    }
    M_variables[name_string] = {type, M_stack_frame_size, modifiable};
    auto size = type.size8();
    M_stack_frame_size += size;
}

std::optional<Variable> SymbolTable::get_variable(const std::string& name) const
{
    auto it = M_variables.find(name);
    if (it != M_variables.end()) return it->second;
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
