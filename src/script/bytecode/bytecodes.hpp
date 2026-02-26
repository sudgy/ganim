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
    10000101 - Use the value at the current stack frame plus another parameter

Bytecode reference:

    111111xx - Test output
 */

namespace ganim::bytecode {
    constexpr auto test_output_byte = std::byte(0b11111100);
    constexpr auto test_output_int = std::byte(0b11111101);
    constexpr auto test_output_uint = std::byte(0b11111110);
}

#endif
