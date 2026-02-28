#include "compiler_state.hpp"

#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace ganim::bytecode;

void CompilerState::write_parameter(byte value)
{
    if (value < byte(128)) {
        bytecode.push_back(value);
    }
    else {
        bytecode.push_back(param_byte1);
        bytecode.push_back(value);
    }
}

void CompilerState::write_parameter(int64_t value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    if (0LL <= value and value < 128LL) {
        bytecode.push_back(bytes[0]);
    }
    else if (-128LL <= value and value < 0LL) {
        bytecode.push_back(param_byte1);
        bytecode.push_back(bytes[0]);
    }
    else if (-0x8000LL <= value and value < 0x8000LL) {
        bytecode.push_back(param_byte2);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
    }
    else if (-0x80000000LL <= value and value < 0x80000000LL) {
        bytecode.push_back(param_byte4);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
    }
    else {
        bytecode.push_back(param_byte8);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
        bytecode.push_back(bytes[4]);
        bytecode.push_back(bytes[5]);
        bytecode.push_back(bytes[6]);
        bytecode.push_back(bytes[7]);
    }
}

void CompilerState::write_parameter(uint64_t value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    if (value and value < 128ULL) {
        bytecode.push_back(bytes[0]);
    }
    else if (value < 256ULL) {
        bytecode.push_back(param_byte1);
        bytecode.push_back(bytes[0]);
    }
    else if (value < 0x10000ULL) {
        bytecode.push_back(param_byte2);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
    }
    else if (value < 0x100000000LL) {
        bytecode.push_back(param_byte4);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
    }
    else {
        bytecode.push_back(param_byte8);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
        bytecode.push_back(bytes[4]);
        bytecode.push_back(bytes[5]);
        bytecode.push_back(bytes[6]);
        bytecode.push_back(bytes[7]);
    }
}

void CompilerState::write_parameter(double value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    bytecode.push_back(param_byte8);
    bytecode.push_back(bytes[0]);
    bytecode.push_back(bytes[1]);
    bytecode.push_back(bytes[2]);
    bytecode.push_back(bytes[3]);
    bytecode.push_back(bytes[4]);
    bytecode.push_back(bytes[5]);
    bytecode.push_back(bytes[6]);
    bytecode.push_back(bytes[7]);
}
