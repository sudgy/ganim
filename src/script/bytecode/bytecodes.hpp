#ifndef GANIM_SCRIPT_BYTECODES_HPP
#define GANIM_SCRIPT_BYTECODES_HPP

#include <cstddef>
#include <cstdint>

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
    000000xx - Push (has parameter)
    00000100 - Pop
    00000101 - Unary minus int (top of stack)
    00000110 - Nop
    00000111 - Unary minus double (top of stack)
    000010XX - Plus
    000011XX - Minus
    000100XX - Mult
    000101XX - Div
    000110XX - Mod (no double)
    00011011 - Unused
    000111XX - And (no double)
    00011111 - Unused
    001000XX - Or (no double)
    00100011 - Unused
    001001XX - Xor (no double)
    00100111 - Unused
    001010XX - Not (no double)
    00101011 - Not bool
    001011XX - Unused
    001100XX - Compare
    00110100 - Jump if equal (short)
    00110101 - Jump if not equal (short)
    00110110 - Jump if less than (short)
    00110111 - Jump if less than or equal (short)
    00111000 - Jump if greater than (short)
    00111001 - Jump if greater than or equal (short)
    00111010 - Jump short
    00111011 - Jump medium
    00111100 - Jump long
    00111101 - Jump to subroutine
    00111110 - Return
    010000XX - Left shift (no double)
    01000011 - Unused
    010001XX - Right shift (no double)
    01000111 - Unused
    01001000 - Move to stack frame
    01001001 - Move to absolute address
 */

namespace ganim {
    using std::byte;
    using std::int64_t;
    using std::uint64_t;
}
namespace ganim::bytecode {
    constexpr auto test_byte = byte(0b11111100);
    constexpr auto test_int = byte(0b11111101);
    constexpr auto test_uint = byte(0b11111110);
    constexpr auto test_double = byte(0b11111111);

    constexpr auto push_byte = byte(0b00000000);
    constexpr auto push_int = byte(0b00000001);
    constexpr auto push_uint = byte(0b00000010);
    constexpr auto push_double = byte(0b00000011);

    constexpr auto pop = byte(0b00000100);
    constexpr auto unary_minus_int = byte(0b00000101);
    constexpr auto nop = byte(0b00000110);
    constexpr auto unary_minus_double = byte(0b00000111);

    constexpr auto plus_byte = byte(0b00001000);
    constexpr auto plus_int = byte(0b00001001);
    constexpr auto plus_uint = byte(0b00001010);
    constexpr auto plus_double = byte(0b00001011);
    constexpr auto minus_byte = byte(0b00001100);
    constexpr auto minus_int = byte(0b00001101);
    constexpr auto minus_uint = byte(0b00001110);
    constexpr auto minus_double = byte(0b00001111);
    constexpr auto mult_byte = byte(0b00010000);
    constexpr auto mult_int = byte(0b00010001);
    constexpr auto mult_uint = byte(0b00010010);
    constexpr auto mult_double = byte(0b00010011);
    constexpr auto div_byte = byte(0b00010100);
    constexpr auto div_int = byte(0b00010101);
    constexpr auto div_uint = byte(0b00010110);
    constexpr auto div_double = byte(0b00010111);
    constexpr auto mod_byte = byte(0b00011000);
    constexpr auto mod_int = byte(0b00011001);
    constexpr auto mod_uint = byte(0b00011010);

    constexpr auto and_byte = byte(0b00011100);
    constexpr auto and_int = byte(0b00011101);
    constexpr auto and_uint = byte(0b00011110);
    constexpr auto or_byte = byte(0b00100000);
    constexpr auto or_int = byte(0b00100001);
    constexpr auto or_uint = byte(0b00100010);
    constexpr auto xor_byte = byte(0b00100100);
    constexpr auto xor_int = byte(0b00100101);
    constexpr auto xor_uint = byte(0b00100110);
    constexpr auto not_byte = byte(0b00101000);
    constexpr auto not_int = byte(0b00101001);
    constexpr auto not_uint = byte(0b00101010);
    constexpr auto not_bool = byte(0b00101011);

    constexpr auto compare_byte = byte(0b00110000);
    constexpr auto compare_int = byte(0b00110001);
    constexpr auto compare_uint = byte(0b00110010);
    constexpr auto compare_double = byte(0b00110011);
    constexpr auto jump_eq = byte(0b00110100);
    constexpr auto jump_neq = byte(0b00110101);
    constexpr auto jump_lt = byte(0b00110110);
    constexpr auto jump_le = byte(0b00110111);
    constexpr auto jump_gt = byte(0b00111000);
    constexpr auto jump_ge = byte(0b00111001);
    constexpr auto jump_short = byte(0b00111010);
    constexpr auto jump_medium = byte(0b00111011);
    constexpr auto jump_long = byte(0b00111100);
    constexpr auto call = byte(0b00111101);
    constexpr auto ret = byte(0b00111110);

    constexpr auto move_stack = byte(0b01001000);
    constexpr auto move_abs = byte(0b01001001);


    constexpr auto param_byte1 = byte(0b10000000);
    constexpr auto param_byte2 = byte(0b10000001);
    constexpr auto param_byte4 = byte(0b10000010);
    constexpr auto param_byte8 = byte(0b10000011);
    constexpr auto param_stack1 = byte(0b10000100);
    constexpr auto param_stack2 = byte(0b10000101);
    constexpr auto param_stack_frame = byte(0b10000110);
}

#endif
