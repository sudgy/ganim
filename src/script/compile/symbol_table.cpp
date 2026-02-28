#include "symbol_table.hpp"

#include "script/script_exception.hpp"

using namespace ganim;

void SymbolTable::add_variable(
    std::string_view name,
    Type type,
    int line_number,
    int column_number
)
{
    auto name_string = std::string(name);
    if (M_variables.contains(name_string)) {
        throw CompileError(line_number, column_number, std::format(
                "A variable by the name \"{}\" already exists.", name));
    }
    M_variables[name_string] = {type, M_stack_frame_size};
    auto size = type.size();
    if (size % 8 == 0) size /= 8;
    else {
        size /= 8;
        ++size;
    }
    M_stack_frame_size += size;
}

std::optional<Variable> SymbolTable::get_variable(const std::string& name) const
{
    auto it = M_variables.find(name);
    if (it != M_variables.end()) return it->second;
    return std::nullopt;
}
