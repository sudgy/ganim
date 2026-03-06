#include "compiler.hpp"

#include <ranges>

#include "script/bytecode/bytecodes.hpp"
#include "script/script_exception.hpp"
#include "script/any_pointer.hpp"
#include "statement/statement.hpp"

using namespace ganim;
using namespace ganim::bytecode;

Compiler::Compiler(const std::vector<syntax::Statement>& ast)
{
    M_stack.emplace_back();
    M_bytecode.reserve(ast.size() * 8);
    for (auto& statement : ast) {
        compile_statement(*this, statement);
    }
    resolve_labels();
}

std::vector<byte> Compiler::take_bytecode()
{
    return std::move(M_bytecode);
}

void Compiler::write_byte(byte value)
{
    M_bytecode.push_back(value);
}

void Compiler::write_parameter(byte value)
{
    if (value < byte(128)) {
        M_bytecode.push_back(value);
    }
    else {
        M_bytecode.push_back(param_byte1);
        M_bytecode.push_back(value);
    }
}

void Compiler::write_parameter(int64_t value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    if (0LL <= value and value < 128LL) {
        M_bytecode.push_back(bytes[0]);
    }
    else if (-128LL <= value and value < 0LL) {
        M_bytecode.push_back(param_byte1);
        M_bytecode.push_back(bytes[0]);
    }
    else if (-0x8000LL <= value and value < 0x8000LL) {
        M_bytecode.push_back(param_byte2);
        M_bytecode.push_back(bytes[0]);
        M_bytecode.push_back(bytes[1]);
    }
    else if (-0x80000000LL <= value and value < 0x80000000LL) {
        M_bytecode.push_back(param_byte4);
        M_bytecode.push_back(bytes[0]);
        M_bytecode.push_back(bytes[1]);
        M_bytecode.push_back(bytes[2]);
        M_bytecode.push_back(bytes[3]);
    }
    else {
        M_bytecode.push_back(param_byte8);
        M_bytecode.push_back(bytes[0]);
        M_bytecode.push_back(bytes[1]);
        M_bytecode.push_back(bytes[2]);
        M_bytecode.push_back(bytes[3]);
        M_bytecode.push_back(bytes[4]);
        M_bytecode.push_back(bytes[5]);
        M_bytecode.push_back(bytes[6]);
        M_bytecode.push_back(bytes[7]);
    }
}

void Compiler::write_parameter(uint64_t value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    if (value and value < 128ULL) {
        M_bytecode.push_back(bytes[0]);
    }
    else if (value < 256ULL) {
        M_bytecode.push_back(param_byte1);
        M_bytecode.push_back(bytes[0]);
    }
    else if (value < 0x10000ULL) {
        M_bytecode.push_back(param_byte2);
        M_bytecode.push_back(bytes[0]);
        M_bytecode.push_back(bytes[1]);
    }
    else if (value < 0x100000000LL) {
        M_bytecode.push_back(param_byte4);
        M_bytecode.push_back(bytes[0]);
        M_bytecode.push_back(bytes[1]);
        M_bytecode.push_back(bytes[2]);
        M_bytecode.push_back(bytes[3]);
    }
    else {
        M_bytecode.push_back(param_byte8);
        M_bytecode.push_back(bytes[0]);
        M_bytecode.push_back(bytes[1]);
        M_bytecode.push_back(bytes[2]);
        M_bytecode.push_back(bytes[3]);
        M_bytecode.push_back(bytes[4]);
        M_bytecode.push_back(bytes[5]);
        M_bytecode.push_back(bytes[6]);
        M_bytecode.push_back(bytes[7]);
    }
}

void Compiler::write_parameter(double value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    M_bytecode.push_back(param_byte8);
    M_bytecode.push_back(bytes[0]);
    M_bytecode.push_back(bytes[1]);
    M_bytecode.push_back(bytes[2]);
    M_bytecode.push_back(bytes[3]);
    M_bytecode.push_back(bytes[4]);
    M_bytecode.push_back(bytes[5]);
    M_bytecode.push_back(bytes[6]);
    M_bytecode.push_back(bytes[7]);
}

void Compiler::write_pop(uint64_t size)
{
    if (size > 0) {
        M_bytecode.push_back(bytecode::pop);
        write_parameter(size);
    }
}

void Compiler::write_jump(LabelType label)
{
    M_jumps.emplace_back(M_bytecode.size(), label);
    M_bytecode.push_back(bytecode::jump_long);
    for (int i = 0; i < 8; ++i) M_bytecode.push_back(byte(0));
}

void Compiler::write_jump(byte jump_bytecode, LabelType label)
{
    M_jumps.emplace_back(M_bytecode.size(), label);
    M_bytecode.push_back(jump_bytecode);
    M_bytecode.push_back(byte(0));
}

Compiler::LabelType Compiler::get_next_label()
{
    return M_label++;
}

void Compiler::resolve_labels()
{
    auto label_map = std::unordered_map<LabelType, uint64_t>();
    for (auto [pos, label] : M_labels) {
        label_map.emplace(label, pos);
    }
    auto new_bytecode = std::vector<byte>();
    new_bytecode.reserve(M_bytecode.size());
    uint64_t pos = 0;
    uint64_t bytes_removed = 0;
    int i1 = 0;
    int i2 = 0;
    auto new_label_map = std::unordered_map<LabelType, uint64_t>();
    while (i1 < ssize(M_labels) or i2 < ssize(M_jumps)) {
        auto pos1 = i1 < ssize(M_labels) ? M_labels[i1].first : -1;
        auto pos2 = i2 < ssize(M_jumps) ? M_jumps[i2].first : -1;
        if (pos1 <= pos2) {
            new_label_map[M_labels[i1].second]
                = M_labels[i1].first - bytes_removed;
            ++i1;
        }
        else {
            std::copy(
                M_bytecode.begin() + pos,
                M_bytecode.begin() + M_jumps[i2].first,
                std::back_inserter(new_bytecode)
            );
            auto this_code = M_bytecode[M_jumps[i2].first];
            if (this_code == jump_long) pos = M_jumps[i2].first + 9;
            else pos = M_jumps[i2].first + 2;
            auto diff = int64_t(label_map[M_jumps[i2].second]) - int64_t(pos);
            M_jumps[i2].first = new_bytecode.size();
            if (this_code != jump_long) {
                new_bytecode.push_back(this_code);
                new_bytecode.push_back(byte(0));
            }
            else if (-0x80 <= diff and diff <= 0x7F) {
                new_bytecode.push_back(jump_short);
                new_bytecode.push_back(byte(0));
                bytes_removed += 7;
            }
            else if (-0x8000 <= diff and diff <= 0x7FFF) {
                new_bytecode.push_back(jump_medium);
                new_bytecode.push_back(byte(0));
                new_bytecode.push_back(byte(0));
                bytes_removed += 6;
            }
            else {
                new_bytecode.push_back(jump_long);
                for (int i = 0; i < 8; ++i) new_bytecode.push_back(byte(0));
            }
            ++i2;
        }
    }
    if (pos < M_bytecode.size()) {
        std::copy(
            M_bytecode.begin() + pos,
            M_bytecode.end(),
            std::back_inserter(new_bytecode)
        );

    }
    for (auto [pos, label] : M_jumps) {
        auto diff = int64_t(new_label_map[label]) - int64_t(pos);
        if (new_bytecode[pos] == jump_medium) {
            diff -= 3;
            auto bytes = reinterpret_cast<byte*>(&diff);
            new_bytecode[pos+1] = bytes[0];
            new_bytecode[pos+2] = bytes[1];
        }
        else if (new_bytecode[pos] == jump_long) {
            auto bytes = reinterpret_cast<byte*>(&new_label_map[label]);
            for (int i = 0; i < 8; ++i) {
                new_bytecode[pos+1+i] = bytes[i];
            }
        }
        else { // Either jump_short or one of the conditional jumps
            diff -= 2;
            auto bytes = reinterpret_cast<byte*>(&diff);
            new_bytecode[pos+1] = bytes[0];
        }
    }
    M_bytecode = std::move(new_bytecode);
}

void Compiler::add_label_reference(LabelType label)
{
    add_label_reference(M_bytecode.size(), label);
}

void Compiler::add_label_reference(std::uint64_t pos, LabelType label)
{
    M_labels.emplace_back(pos, label);
}
void Compiler::set_loop_labels(LabelType continue_label, LabelType break_label)
{
    auto& table = M_stack.back();
    table.M_continue_label = continue_label;
    table.M_break_label = break_label;
}

std::optional<Compiler::LabelType> Compiler::get_continue_label() const
{
    for (auto& table : std::views::reverse(M_stack)) {
        if (table.M_continue_label != -1) return table.M_continue_label;
    }
    return std::nullopt;
}

std::optional<Compiler::LabelType> Compiler::get_break_label() const
{
    for (auto& table : std::views::reverse(M_stack)) {
        if (table.M_break_label != -1) return table.M_break_label;
    }
    return std::nullopt;
}

void Compiler::add_variable(
    std::string_view name,
    Type type,
    bool modifiable,
    int line_number,
    int column_number
)
{
    auto& table = M_stack.back();
    auto name_string = std::string(name);
    if (table.M_variables.contains(name_string)) {
        throw CompileError(line_number, column_number, std::format(
                "A variable by the name \"{}\" already exists.", name));
    }
    table.M_variables[name_string]
        = {type, table.M_stack_frame_size, modifiable};
    auto size = type.size8();
    table.M_stack_frame_size += size;
}

std::optional<Variable>
Compiler::get_variable(const std::string& name) const
{
    for (auto& table : std::views::reverse(M_stack)) {
        auto it = table.M_variables.find(name);
        if (it != table.M_variables.end()) return it->second;
    }
    return std::nullopt;
}

Type Compiler::get_type(const syntax::Type& type) const
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

void Compiler::push_symbols()
{
    auto& new_table = M_stack.emplace_back();
    auto& current_table = M_stack[M_stack.size()-2];
    new_table.M_stack_frame_size = current_table.M_stack_frame_size;
}

uint64_t Compiler::pop_symbols()
{
    auto old_frame_size = M_stack.back().M_stack_frame_size;
    M_stack.pop_back();
    auto new_frame_size = M_stack.back().M_stack_frame_size;
    return old_frame_size - new_frame_size;
}

uint64_t Compiler::get_loop_pop_size()
{
    auto old_frame_size = M_stack.back().M_stack_frame_size;
    int i = ssize(M_stack);
    for (auto& table : std::views::reverse(M_stack)) {
        --i;
        if (table.M_break_label != -1) {
            --i;
            break;
        }
    }
    auto new_frame_size = M_stack[i].M_stack_frame_size;
    return old_frame_size - new_frame_size;
}
