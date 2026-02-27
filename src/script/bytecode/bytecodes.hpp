#ifndef GANIM_SCRIPT_BYTECODES_HPP
#define GANIM_SCRIPT_BYTECODES_HPP

#include <cstddef>

/*
There are four data types known to ganimscript bytecode:
    byte (8-bit)
    int (64-bit)
    uint (64-bit)
    double (64-bit)
These four types are represented by the bit patterns 00, 01, 10, and 11
respectively.

Parameter reference:
    0xxxxxxx - Directly use a number 0-127
    10000000 - The following byte has the value
                    For int, this byte is signed (and should always be negative
                        since otherwise it would have been better to use a
                        direct number)
                    For uint and byte, this byte is unsigned
    10000001 - The following two bytes have the value
                    For int, this value is signed
                    For uint, this value is unsigned
    10000010 - The following four bytes have the value
                    For int, this value is signed
                    For uint, this value is unsigned
    10000011 - The following eight bytes have the value
                    For int, this value is signed
                    For uint, this value is unsigned
                    This is the only possibility for a constant of type double
    10000100 - Use the value on the top of the stack
    10000101 - Use the value right below the top of the stack
    10000110 - Use the value at the current stack frame plus another parameter

Bytecode reference:

    111111xx - Test output
    000000xx - Push
    00000100 - Pop
 */

namespace ganim::bytecode {
    constexpr auto test_byte = std::byte(0b11111100);
    constexpr auto test_int = std::byte(0b11111101);
    constexpr auto test_uint = std::byte(0b11111110);
    constexpr auto test_double = std::byte(0b11111111);

    constexpr auto push_byte = std::byte(0b00000000);
    constexpr auto push_int = std::byte(0b00000001);
    constexpr auto push_uint = std::byte(0b00000010);
    constexpr auto push_double = std::byte(0b00000011);

    constexpr auto pop = std::byte(0b00000100);

    constexpr auto param_byte1 = std::byte(0b10000000);
    constexpr auto param_byte2 = std::byte(0b10000001);
    constexpr auto param_byte4 = std::byte(0b10000010);
    constexpr auto param_byte8 = std::byte(0b10000011);
    constexpr auto param_stack1 = std::byte(0b10000100);
    constexpr auto param_stack2 = std::byte(0b10000101);
}

#endif
