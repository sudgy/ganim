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
        default:
            throw std::runtime_error("Illegal instruction");
        }
        ++M_program_counter;
    }
}

std::byte Interpreter::read_byte_parameter()
{
    ++M_program_counter;
    if (M_program_counter >= M_code.size()) {
        throw std::runtime_error("Malformed instruction");
    }
    auto parameter = M_code[M_program_counter];
    if (parameter < std::byte(128)) return parameter;
    if (parameter == std::byte(0b10000000)) {
        ++M_program_counter;
        if (M_program_counter >= M_code.size()) {
            throw std::runtime_error("Malformed instruction");
        }
        return M_code[M_program_counter];
    }
    throw std::runtime_error("Malformed instruction");
}
