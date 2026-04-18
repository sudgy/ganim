#include "disassemble.hpp"

#include "bytecodes.hpp"

#include <unordered_map>
#include <cstring>

using namespace ganim;
using namespace bytecode;

namespace {
    struct Disassembler {
        const std::vector<std::byte>& code;
        int i;
        struct Instruction {
            std::string description;
            uint64_t address;
        };
        std::vector<Instruction> result;
        std::unordered_map<uint64_t, std::string> labels;
        void disassemble()
        {
            while (i < ssize(code)) {
                auto a = i;
                auto opcode = code[i];
                ++i;
                switch (opcode) {
                case push_byte:
                    result.emplace_back("push_byte " + read_byte_parameter(), a);
                    break;
                case push_int:
                    result.emplace_back("push_int " + read_int_parameter(), a);
                    break;
                case push_uint:
                    result.emplace_back("push_uint " + read_uint_parameter(), a);
                    break;
                case push_double:
                    result.emplace_back("push_double " + read_double_parameter(), a);
                    break;
                case pop:
                    result.emplace_back("pop " + read_uint_parameter(), a);
                    break;
                case unary_minus_int:
                    result.emplace_back("unary_minus_int", a);
                    break;
                case nop:
                    result.emplace_back("nop", a);
                    break;
                case unary_minus_double:
                    result.emplace_back("unary_minus_double", a);
                    break;
                case plus_byte:
                    result.emplace_back("plus_byte", a);
                    break;
                case plus_int:
                    result.emplace_back("plus_int", a);
                    break;
                case plus_uint:
                    result.emplace_back("plus_uint", a);
                    break;
                case plus_double:
                    result.emplace_back("plus_double", a);
                    break;
                case minus_byte:
                    result.emplace_back("minus_byte", a);
                    break;
                case minus_int:
                    result.emplace_back("minus_int", a);
                    break;
                case minus_uint:
                    result.emplace_back("minus_uint", a);
                    break;
                case minus_double:
                    result.emplace_back("minus_double", a);
                    break;
                case mult_byte:
                    result.emplace_back("mult_byte", a);
                    break;
                case mult_int:
                    result.emplace_back("mult_int", a);
                    break;
                case mult_uint:
                    result.emplace_back("mult_uint", a);
                    break;
                case mult_double:
                    result.emplace_back("mult_double", a);
                    break;
                case div_byte:
                    result.emplace_back("div_byte", a);
                    break;
                case div_int:
                    result.emplace_back("div_int", a);
                    break;
                case div_uint:
                    result.emplace_back("div_uint", a);
                    break;
                case div_double:
                    result.emplace_back("div_double", a);
                    break;
                case mod_byte:
                    result.emplace_back("mod_byte", a);
                    break;
                case mod_int:
                    result.emplace_back("mod_int", a);
                    break;
                case mod_uint:
                    result.emplace_back("mod_uint", a);
                    break;
                case and_byte:
                    result.emplace_back("and_byte", a);
                    break;
                case or_byte:
                    result.emplace_back("or_byte", a);
                    break;
                case xor_byte:
                    result.emplace_back("xor_byte", a);
                    break;
                case not_bool:
                    result.emplace_back("not_bool", a);
                    break;
                case compare_byte:
                    result.emplace_back("compare_byte", a);
                    break;
                case compare_int:
                    result.emplace_back("compare_int", a);
                    break;
                case compare_uint:
                    result.emplace_back("compare_uint", a);
                    break;
                case compare_double:
                    result.emplace_back("compare_double", a);
                    break;
                case jump_eq:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump_eq " + std::string(label), a);
                    break;
                }
                case jump_neq:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump_neq " + std::string(label), a);
                    break;
                }
                case jump_lt:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump_lt " + std::string(label), a);
                    break;
                }
                case jump_le:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump_le " + std::string(label), a);
                    break;
                }
                case jump_gt:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump_gt " + std::string(label), a);
                    break;
                }
                case jump_ge:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump_ge " + std::string(label), a);
                    break;
                }
                case jump_short:
                {
                    auto label = add_label(i + uint64_t(code[i]) + 1);
                    ++i;
                    result.emplace_back("jump " + std::string(label), a);
                    break;
                }
                case jump_medium:
                {
                    auto amount = std::int16_t();
                    std::memcpy(&amount, &code[i], 2);
                    i += 2;
                    auto label = add_label(i + amount);
                    result.emplace_back("jump " + std::string(label), a);
                    break;
                }
                case jump_long:
                {
                    auto address = std::uint64_t();
                    std::memcpy(&address, &code[i], 8);
                    i += 8;
                    auto label = add_label(address);
                    result.emplace_back("jump " + std::string(label), a);
                    break;
                }
                case enter:
                    result.emplace_back("enter " + read_uint_parameter(), a);
                    break;
                case leave:
                    result.emplace_back("leave " + read_uint_parameter(), a);
                    break;
                case ret:
                    result.emplace_back("ret", a);
                    break;
                case call_medium:
                {
                    auto amount = std::int16_t();
                    std::memcpy(&amount, &code[i], 2);
                    i += 2;
                    auto label = add_label(i + amount);
                    result.emplace_back("call " + std::string(label), a);
                    break;
                }
                case call_param:
                {
                    auto location = read_uint_parameter();
                    if (location[0] >= '0' and location[0] <= '9') {
                        auto label = add_label(std::stoi(location));
                        result.emplace_back("call " + std::string(label), a);
                    }
                    else {
                        result.emplace_back("call " + location, a);
                    }
                    break;
                }
                case move_stack:
                    result.emplace_back(
                        "move stack[ " + read_uint_parameter() + "]", a
                    );
                    break;
                case move_global:
                    result.emplace_back(
                        "move global[ " + read_uint_parameter() + "]", a
                    );
                    break;
                case test_byte:
                    result.emplace_back("test_byte");
                    break;
                case test_int:
                    result.emplace_back("test_int");
                    break;
                case test_uint:
                    result.emplace_back("test_uint");
                    break;
                case test_double:
                    result.emplace_back("test_double");
                    break;
                default:
                    result.emplace_back("INVALID");
                    break;
                }
            }
        }
        std::string_view add_label(uint64_t address)
        {
            auto [it, result] = labels.emplace(address, "");
            if (result) {
                it->second = std::format("label{}", labels.size());
            }
            return it->second;
        }
        std::string read_byte_parameter()
        {
            auto parameter = code[i];
            ++i;
            if (parameter < byte(128)) {
                return std::to_string(int(parameter));
            }
            else switch (parameter) {
            case param_byte1:
                ++i;
                return std::to_string(int(code[i-1]));
            case param_stack1:
                return "stack1";
            case param_stack2:
                return "stack2";
            case param_stack_frame:
                return "stack_frame[" + read_uint_parameter() + "]";
            case param_global:
                return "global[" + read_uint_parameter() + "]";
            default:
                return "INVALID";
            }
        }
        std::string read_int_parameter()
        {
            auto parameter = code[i];
            ++i;
            auto result = int64_t(0);
            auto bytes = reinterpret_cast<byte*>(&result);
            if (parameter < byte(128)) {
                bytes[0] = parameter;
            }
            else switch (parameter) {
            case param_byte1:
                bytes[0] = code[i];
                if (bytes[0] >= byte(128)) {
                    for (int i = 1; i < 8; ++i) {
                        bytes[i] = byte(0xFF);
                    }
                }
                ++i;
                break;
            case param_byte2:
                bytes[0] = code[i];
                bytes[1] = code[i+1];
                if (bytes[1] >= byte(128)) {
                    for (int i = 2; i < 8; ++i) {
                        bytes[i] = byte(0xFF);
                    }
                }
                i += 2;
                break;
            case param_byte4:
                bytes[0] = code[i];
                bytes[1] = code[i+1];
                bytes[2] = code[i+2];
                bytes[3] = code[i+3];
                if (bytes[3] >= byte(128)) {
                    for (int i = 4; i < 8; ++i) {
                        bytes[i] = byte(0xFF);
                    }
                }
                i += 4;
                break;
            case param_byte8:
                bytes[0] = code[i+0];
                bytes[1] = code[i+1];
                bytes[2] = code[i+2];
                bytes[3] = code[i+3];
                bytes[4] = code[i+4];
                bytes[5] = code[i+5];
                bytes[6] = code[i+6];
                bytes[7] = code[i+7];
                i += 8;
                break;
            case param_stack1:
                return "stack1";
            case param_stack2:
                return "stack2";
            case param_stack_frame:
                return "stack_frame[" + read_uint_parameter() + "]";
            case param_global:
                return "global[" + read_uint_parameter() + "]";
            default:
                return "INVALID";
            }
            return std::to_string(result);
        }
        std::string read_uint_parameter()
        {
            auto parameter = code[i];
            ++i;
            auto result = uint64_t(0);
            auto bytes = reinterpret_cast<byte*>(&result);
            if (parameter < byte(128)) {
                bytes[0] = parameter;
            }
            else switch (parameter) {
            case param_byte1:
                bytes[0] = code[i];
                ++i;
                break;
            case param_byte2:
                bytes[0] = code[i];
                bytes[1] = code[i+1];
                i += 2;
                break;
            case param_byte4:
                bytes[0] = code[i];
                bytes[1] = code[i+1];
                bytes[2] = code[i+2];
                bytes[3] = code[i+3];
                i += 4;
                break;
            case param_byte8:
                bytes[0] = code[i+0];
                bytes[1] = code[i+1];
                bytes[2] = code[i+2];
                bytes[3] = code[i+3];
                bytes[4] = code[i+4];
                bytes[5] = code[i+5];
                bytes[6] = code[i+6];
                bytes[7] = code[i+7];
                i += 8;
                break;
            case param_stack1:
                return "stack1";
            case param_stack2:
                return "stack2";
            case param_stack_frame:
                return "stack_frame[" + read_uint_parameter() + "]";
            case param_global:
                return "global[" + read_uint_parameter() + "]";
            default:
                return "INVALID";
            }
            return std::to_string(result);
        }
        std::string read_double_parameter()
        {
            auto parameter = code[i];
            ++i;
            auto result = double(0);
            auto bytes = reinterpret_cast<byte*>(&result);
            switch (parameter) {
            case param_byte8:
                bytes[0] = code[i+0];
                bytes[1] = code[i+1];
                bytes[2] = code[i+2];
                bytes[3] = code[i+3];
                bytes[4] = code[i+4];
                bytes[5] = code[i+5];
                bytes[6] = code[i+6];
                bytes[7] = code[i+7];
                i += 8;
                break;
            case param_stack1:
                return "stack1";
            case param_stack2:
                return "stack2";
            case param_stack_frame:
                return "stack_frame[" + read_uint_parameter() + "]";
            case param_global:
                return "global[" + read_uint_parameter() + "]";
            default:
                return "INVALID";
            }
            return std::to_string(result);
        }
    };
}

void ganim::disassemble(
    const std::vector<std::byte>& code,
    std::ostream& output
)
{
    auto disassembler = Disassembler{code, 0, {}, {}};
    disassembler.disassemble();
    for (auto& instruction : disassembler.result) {
        auto it = disassembler.labels.find(instruction.address);
        if (it != disassembler.labels.end()) {
            output << it->second << ":\n";
        }
        output << "    " << instruction.description << "\n";
    }
    auto it = disassembler.labels.find(code.size());
    if (it != disassembler.labels.end()) {
        output << it->second << ":\n";
    }
}
