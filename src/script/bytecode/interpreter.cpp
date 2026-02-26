#include "interpreter.hpp"

#include <stdexcept>

#include "bytecodes.hpp"

using namespace ganim;

Interpreter::Interpreter(std::vector<std::byte> code)
:   M_code(code) {}


void Interpreter::execute()
{
    using namespace bytecode;
    while (M_program_counter < M_code.size()) {
        auto opcode = M_code[M_program_counter];
        switch (opcode) {
        case test_output_byte:
            M_test_output.emplace_back(read_byte_parameter());
            break;
        case test_output_int:
            M_test_output.emplace_back(read_int_parameter());
            break;
        default:
            throw std::runtime_error("Illegal instruction");
        }
        ++M_program_counter;
    }
}

std::byte Interpreter::read_byte_parameter()
{
    safe_increase_program_counter();
    auto parameter = M_code[M_program_counter];
    if (parameter < std::byte(128)) return parameter;
    if (parameter == std::byte(0b10000000)) {
        safe_increase_program_counter();
        return M_code[M_program_counter];
    }
    throw std::runtime_error("Malformed instruction");
}

std::int64_t Interpreter::read_int_parameter()
{
    safe_increase_program_counter();
    auto result = std::int64_t(0);
    auto bytes = reinterpret_cast<std::byte*>(&result);
    auto parameter = M_code[M_program_counter];
    if (parameter < std::byte(128)) {
        bytes[0] = parameter;
    }
    else if (parameter == std::byte(0b10000000)) {
        safe_increase_program_counter();
        bytes[0] = M_code[M_program_counter];
        if (bytes[0] >= std::byte(128)) {
            for (int i = 1; i < 8; ++i) {
                bytes[i] = std::byte(0xFF);
            }
        }
    }
    else if (parameter == std::byte(0b10000001)) {
        safe_increase_program_counter();
        bytes[0] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[1] = M_code[M_program_counter];
        if (bytes[0] >= std::byte(128)) {
            for (int i = 2; i < 8; ++i) {
                bytes[i] = std::byte(0xFF);
            }
        }
    }
    else if (parameter == std::byte(0b10000010)) {
        safe_increase_program_counter();
        bytes[0] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[1] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[2] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[3] = M_code[M_program_counter];
        if (bytes[0] >= std::byte(128)) {
            for (int i = 4; i < 8; ++i) {
                bytes[i] = std::byte(0xFF);
            }
        }
    }
    else if (parameter == std::byte(0b10000011)) {
        safe_increase_program_counter();
        bytes[0] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[1] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[2] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[3] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[4] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[5] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[6] = M_code[M_program_counter];
        safe_increase_program_counter();
        bytes[7] = M_code[M_program_counter];
    }
    else throw std::runtime_error("Malformed instruction");
    return result;
}

void Interpreter::safe_increase_program_counter()
{
    ++M_program_counter;
    if (M_program_counter >= M_code.size()) {
        throw std::runtime_error("Malformed instruction");
    }
}
