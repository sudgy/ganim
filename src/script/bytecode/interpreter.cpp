#include "interpreter.hpp"

#include <stdexcept>
#include <cstring>

#include "bytecodes.hpp"

using namespace ganim;

Interpreter::Interpreter(std::vector<byte> code)
:   M_code(code) {}

using namespace bytecode;

void Interpreter::execute()
{
    while (M_program_counter < M_code.size()) {
        auto opcode = M_code[M_program_counter];
        switch (opcode) {
        case push_byte:
        {
            auto byte = read_byte_parameter();
            push_bytes({&byte, 1});
            break;
        }
        case push_int:
        {
            auto bytes = read_int_parameter();
            push_bytes({reinterpret_cast<byte*>(&bytes), 8});
            break;
        }
        case push_uint:
        {
            auto bytes = read_uint_parameter();
            push_bytes({reinterpret_cast<byte*>(&bytes), 8});
            break;
        }
        case push_double:
        {
            auto bytes = read_double_parameter();
            push_bytes({reinterpret_cast<byte*>(&bytes), 8});
            break;
        }
        case bytecode::pop:
        {
            pop(read_uint_parameter());
            break;
        }
        case unary_minus_int:
        {
            auto& val = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            val = -val;
            break;
        }
        case nop:
            break;
        case unary_minus_double:
        {
            auto& val = reinterpret_cast<double&>(M_stack[M_stack.size()-8]);
            val = -val;
            break;
        }
        case plus_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = byte((unsigned char)val1 + (unsigned char)val2);
            break;
        }
        case plus_int:
        {
            auto val1 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 8])
                = val1 + val2;
            break;
        }
        case plus_uint:
        {
            auto val1 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 8])
                = val1 + val2;
            break;
        }
        case plus_double:
        {
            auto val1 = reinterpret_cast<double&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<double&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<double&>(M_stack[M_stack.size() - 8])
                = val1 + val2;
            break;
        }
        case minus_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = byte((unsigned char)val1 - (unsigned char)val2);
            break;
        }
        case minus_int:
        {
            auto val1 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 8])
                = val1 - val2;
            break;
        }
        case minus_uint:
        {
            auto val1 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 8])
                = val1 - val2;
            break;
        }
        case minus_double:
        {
            auto val1 = reinterpret_cast<double&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<double&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<double&>(M_stack[M_stack.size() - 8])
                = val1 - val2;
            break;
        }
        case mult_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = byte((unsigned char)val1 * (unsigned char)val2);
            break;
        }
        case mult_int:
        {
            auto val1 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 8])
                = val1 * val2;
            break;
        }
        case mult_uint:
        {
            auto val1 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 8])
                = val1 * val2;
            break;
        }
        case mult_double:
        {
            auto val1 = reinterpret_cast<double&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<double&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<double&>(M_stack[M_stack.size() - 8])
                = val1 * val2;
            break;
        }
        case div_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = byte((unsigned char)val1 / (unsigned char)val2);
            break;
        }
        case div_int:
        {
            auto val1 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 8])
                = val1 / val2;
            break;
        }
        case div_uint:
        {
            auto val1 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 8])
                = val1 / val2;
            break;
        }
        case div_double:
        {
            auto val1 = reinterpret_cast<double&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<double&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<double&>(M_stack[M_stack.size() - 8])
                = val1 / val2;
            break;
        }
        case mod_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = byte((unsigned char)val1 % (unsigned char)val2);
            break;
        }
        case mod_int:
        {
            auto val1 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 8])
                = val1 % val2;
            break;
        }
        case mod_uint:
        {
            auto val1 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 8])
                = val1 % val2;
            break;
        }
        case and_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8] = val1 & val2;
            break;
        }
        case or_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8] = val1 | val2;
            break;
        }
        case xor_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8] = val1 ^ val2;
            break;
        }
        case not_bool:
        {
            auto val = M_stack[M_stack.size() - 8];
            M_stack[M_stack.size() - 8] = val == byte(0) ? byte(1) : byte(0);
            break;
        }
        case compare_byte:
        {
            auto val1 = M_stack[M_stack.size() - 16];
            auto val2 = M_stack[M_stack.size() - 8];
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = (unsigned char)val1 < (unsigned char)val2 ? byte(0xFF) :
                    val1 == val2 ? byte(0) : byte(1);
            break;
        }
        case compare_int:
        {
            auto val1 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<int64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = val1 < val2 ? byte(0xFF) :
                  val1 == val2 ? byte(0) : byte(1);
            break;
        }
        case compare_uint:
        {
            auto val1 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<uint64_t&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = val1 < val2 ? byte(0xFF) :
                  val1 == val2 ? byte(0) : byte(1);
            break;
        }
        case compare_double:
        {
            auto val1 = reinterpret_cast<double&>(M_stack[M_stack.size()-16]);
            auto val2 = reinterpret_cast<double&>(M_stack[M_stack.size()-8]);
            M_stack.resize(M_stack.size() - 8);
            M_stack[M_stack.size() - 8]
                = val1 < val2 ? byte(0xFF) :
                  val1 == val2 ? byte(0) :
                  val1 > val2 ? byte(1) : byte(2);
            break;
        }
        case jump_eq:
            safe_increase_program_counter();
            if (M_stack[M_stack.size() - 8] == byte(0)) {
                M_program_counter += char(M_code[M_program_counter]);
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        case jump_neq:
            safe_increase_program_counter();
            if (M_stack[M_stack.size() - 8] != byte(0)) {
                M_program_counter += char(M_code[M_program_counter]);
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        case jump_lt:
            safe_increase_program_counter();
            if (M_stack[M_stack.size() - 8] == byte(0xFF)) {
                M_program_counter += char(M_code[M_program_counter]);
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        case jump_le:
            safe_increase_program_counter();
            if (M_stack[M_stack.size() - 8] == byte(0xFF) or
                M_stack[M_stack.size() - 8] == byte(0))
            {
                M_program_counter += char(M_code[M_program_counter]);
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        case jump_gt:
            safe_increase_program_counter();
            if (M_stack[M_stack.size() - 8] == byte(1)) {
                M_program_counter += char(M_code[M_program_counter]);
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        case jump_ge:
            safe_increase_program_counter();
            if (M_stack[M_stack.size() - 8] == byte(1) or
                M_stack[M_stack.size() - 8] == byte(0))
            {
                M_program_counter += char(M_code[M_program_counter]);
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        case jump_short:
            safe_increase_program_counter();
            M_program_counter += char(M_code[M_program_counter]);
            break;
        case jump_medium:
        {
            safe_increase_program_counter(2);
            auto amount = std::int16_t();
            std::memcpy(&amount, &M_code[M_program_counter-1], 2);
            M_program_counter += amount;
            break;
        }
        case jump_long:
            safe_increase_program_counter(8);
            std::memcpy(&M_program_counter, &M_code[M_program_counter-7], 8);
            --M_program_counter;
            break;
        case move_stack:
        {
            auto size = read_uint_parameter();
            for (int i = 0; i < 8; ++i) {
                M_stack[M_stack_frame + size*8+i]
                    = M_stack[M_stack.size() - 8+i];
            }
            M_stack.resize(M_stack.size() - 8);
            break;
        }
        case test_byte:
            M_test_output.emplace_back(get_stack_byte());
            break;
        case test_int:
            M_test_output.emplace_back(get_stack_int());
            break;
        case test_uint:
            M_test_output.emplace_back(get_stack_uint());
            break;
        case test_double:
            M_test_output.emplace_back(get_stack_double());
            break;
        default:
            throw std::runtime_error("Illegal instruction");
        }
        ++M_program_counter;
    }
}

byte Interpreter::read_byte_parameter()
{
    safe_increase_program_counter();
    auto parameter = M_code[M_program_counter];
    if (parameter < byte(128)) return parameter;
    switch (parameter) {
    case param_byte1:
        safe_increase_program_counter();
        return M_code[M_program_counter];
    case param_stack1:
        return M_stack[M_stack.size() - 8];
    case param_stack2:
        return M_stack[M_stack.size() - 16];
    case param_stack_frame:
        return M_stack[M_stack_frame + read_uint_parameter()*8];
    default:
        throw std::runtime_error("Malformed instruction");
    }
}

int64_t Interpreter::read_int_parameter()
{
    safe_increase_program_counter();
    auto result = int64_t(0);
    auto bytes = reinterpret_cast<byte*>(&result);
    auto parameter = M_code[M_program_counter];
    if (parameter < byte(128)) {
        bytes[0] = parameter;
    }
    else switch (parameter) {
    case param_byte1:
        safe_increase_program_counter();
        bytes[0] = M_code[M_program_counter];
        if (bytes[0] >= byte(128)) {
            for (int i = 1; i < 8; ++i) {
                bytes[i] = byte(0xFF);
            }
        }
        break;
    case param_byte2:
        safe_increase_program_counter(2);
        bytes[0] = M_code[M_program_counter-1];
        bytes[1] = M_code[M_program_counter];
        if (bytes[1] >= byte(128)) {
            for (int i = 2; i < 8; ++i) {
                bytes[i] = byte(0xFF);
            }
        }
        break;
    case param_byte4:
        safe_increase_program_counter(4);
        bytes[0] = M_code[M_program_counter-3];
        bytes[1] = M_code[M_program_counter-2];
        bytes[2] = M_code[M_program_counter-1];
        bytes[3] = M_code[M_program_counter];
        if (bytes[3] >= byte(128)) {
            for (int i = 4; i < 8; ++i) {
                bytes[i] = byte(0xFF);
            }
        }
        break;
    case param_byte8:
        safe_increase_program_counter(8);
        bytes[0] = M_code[M_program_counter-7];
        bytes[1] = M_code[M_program_counter-6];
        bytes[2] = M_code[M_program_counter-5];
        bytes[3] = M_code[M_program_counter-4];
        bytes[4] = M_code[M_program_counter-3];
        bytes[5] = M_code[M_program_counter-2];
        bytes[6] = M_code[M_program_counter-1];
        bytes[7] = M_code[M_program_counter];
        break;
    case param_stack1:
        return reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 8]);
    case param_stack2:
        return reinterpret_cast<int64_t&>(M_stack[M_stack.size() - 16]);
    case param_stack_frame:
        return reinterpret_cast<int64_t&>(
            M_stack[M_stack_frame + read_uint_parameter()*8]);
    default:
        throw std::runtime_error("Malformed instruction");
    }
    return result;
}

uint64_t Interpreter::read_uint_parameter()
{
    safe_increase_program_counter();
    auto result = uint64_t(0);
    auto bytes = reinterpret_cast<byte*>(&result);
    auto parameter = M_code[M_program_counter];
    if (parameter < byte(128)) {
        bytes[0] = parameter;
    }
    else switch (parameter) {
    case param_byte1:
        safe_increase_program_counter();
        bytes[0] = M_code[M_program_counter];
        break;
    case param_byte2:
        safe_increase_program_counter(2);
        bytes[0] = M_code[M_program_counter-1];
        bytes[1] = M_code[M_program_counter];
        break;
    case param_byte4:
        safe_increase_program_counter(4);
        bytes[0] = M_code[M_program_counter-3];
        bytes[1] = M_code[M_program_counter-2];
        bytes[2] = M_code[M_program_counter-1];
        bytes[3] = M_code[M_program_counter];
        break;
    case param_byte8:
        safe_increase_program_counter(8);
        bytes[0] = M_code[M_program_counter-7];
        bytes[1] = M_code[M_program_counter-6];
        bytes[2] = M_code[M_program_counter-5];
        bytes[3] = M_code[M_program_counter-4];
        bytes[4] = M_code[M_program_counter-3];
        bytes[5] = M_code[M_program_counter-2];
        bytes[6] = M_code[M_program_counter-1];
        bytes[7] = M_code[M_program_counter];
        break;
    case param_stack1:
        return reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 8]);
    case param_stack2:
        return reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - 16]);
    case param_stack_frame:
        return reinterpret_cast<uint64_t&>(
            M_stack[M_stack_frame + read_uint_parameter()*8]);
    default:
        throw std::runtime_error("Malformed instruction");
    }
    return result;
}

double Interpreter::read_double_parameter()
{
    safe_increase_program_counter();
    auto result = double(0);
    auto bytes = reinterpret_cast<byte*>(&result);
    auto parameter = M_code[M_program_counter];
    switch (parameter) {
    case param_byte8:
        safe_increase_program_counter(8);
        bytes[0] = M_code[M_program_counter-7];
        bytes[1] = M_code[M_program_counter-6];
        bytes[2] = M_code[M_program_counter-5];
        bytes[3] = M_code[M_program_counter-4];
        bytes[4] = M_code[M_program_counter-3];
        bytes[5] = M_code[M_program_counter-2];
        bytes[6] = M_code[M_program_counter-1];
        bytes[7] = M_code[M_program_counter];
        break;
    case param_stack1:
        return reinterpret_cast<double&>(M_stack[M_stack.size() - 8]);
    case param_stack2:
        return reinterpret_cast<double&>(M_stack[M_stack.size() - 16]);
    case param_stack_frame:
        return reinterpret_cast<double&>(
            M_stack[M_stack_frame + read_uint_parameter()*8]);
    default:
        throw std::runtime_error("Malformed instruction");
    }
    return result;
}

byte Interpreter::get_stack_byte(int from_top)
{
    return M_stack[M_stack.size() - from_top*8];
}

int64_t Interpreter::get_stack_int(int from_top)
{
    return reinterpret_cast<int64_t&>(M_stack[M_stack.size() - from_top*8]);
}

uint64_t Interpreter::get_stack_uint(int from_top)
{
    return reinterpret_cast<uint64_t&>(M_stack[M_stack.size() - from_top*8]);
}

double Interpreter::get_stack_double(int from_top)
{
    return reinterpret_cast<double&>(M_stack[M_stack.size() - from_top*8]);
}

void Interpreter::push_bytes(std::span<byte> bytes)
{
    auto size_plus = bytes.size();
    auto padding = size_plus % 8;
    if (padding != 0) {
        padding = 8 - padding;
        size_plus += padding;
    }
    auto start = M_stack.size();
    M_stack.resize(M_stack.size() + size_plus);
    for (auto i = 0UZ; i < bytes.size(); ++i) {
        M_stack[start + i] = bytes[i];
    }
}

void Interpreter::pop(std::size_t amount)
{
    auto byte_amount = amount * 8;
    if (byte_amount > M_stack.size()) {
        throw std::runtime_error("Stack underflow");
    }
    M_stack.resize(M_stack.size() - byte_amount);
}

void Interpreter::safe_increase_program_counter(int amount)
{
    M_program_counter += amount;
    if (M_program_counter >= M_code.size()) {
        throw std::runtime_error("Malformed instruction");
    }
}
