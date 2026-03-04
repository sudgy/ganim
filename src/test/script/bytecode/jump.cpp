#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Intepreter jumps", "[script]") {
    auto code = std::vector<byte>{
        push_int, byte(2), test_int, jump_short, byte(25), nop, nop, nop, nop,
            nop,
        push_int, byte(3), test_int, nop, nop, nop, nop, nop, nop, nop,
        jump_long, byte(70), byte(0), byte(0), byte(0), byte(0), byte(0),
            byte(0), byte(0), nop,
        push_int, byte(4), test_int, nop, jump_medium, byte(23), byte(0), nop,
            nop, nop,
        push_int, byte(5), test_int, nop, nop, nop, nop, nop, nop, nop,
        jump_long, byte(80), byte(0), byte(0), byte(0), byte(0), byte(0),
            byte(0), byte(0), nop,
        push_int, byte(6), test_int, jump_short, byte(char(-55)), nop, nop, nop,
            nop, nop,
        push_int, byte(7), test_int, jump_medium, byte(char(-36)), byte(0xFF),
            nop, nop, nop, nop,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 6);
    REQUIRE(get<int64_t>(output[0]) == 2);
    REQUIRE(get<int64_t>(output[1]) == 4);
    REQUIRE(get<int64_t>(output[2]) == 6);
    REQUIRE(get<int64_t>(output[3]) == 3);
    REQUIRE(get<int64_t>(output[4]) == 7);
    REQUIRE(get<int64_t>(output[5]) == 5);
}

TEST_CASE("Interpreter conditional jumps bytes", "[script]") {
    auto code = std::vector<byte>{
        push_byte, byte(2), push_byte, byte(3),
        compare_byte, jump_eq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_byte, byte(2), push_byte, byte(2),
        compare_byte, jump_eq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_byte, byte(2), push_byte, byte(3),
        compare_byte, jump_neq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_byte, byte(2), push_byte, byte(2),
        compare_byte, jump_neq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_byte, byte(2), push_byte, byte(3),
        compare_byte, jump_lt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_byte, byte(2), push_byte, byte(2),
        compare_byte, jump_lt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_byte, byte(3), push_byte, byte(2),
        compare_byte, jump_le, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_byte, byte(2), push_byte, byte(2),
        compare_byte, jump_le, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_byte, byte(3), push_byte, byte(2),
        compare_byte, jump_gt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_byte, byte(2), push_byte, byte(2),
        compare_byte, jump_gt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_byte, byte(2), push_byte, byte(3),
        compare_byte, jump_ge, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_byte, byte(2), push_byte, byte(2),
        compare_byte, jump_ge, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 18);
    REQUIRE(get<int64_t>(output[0]) == 10);
    REQUIRE(get<int64_t>(output[1]) == 11);
    REQUIRE(get<int64_t>(output[2]) == 13);
    REQUIRE(get<int64_t>(output[3]) == 11);
    REQUIRE(get<int64_t>(output[4]) == 12);
    REQUIRE(get<int64_t>(output[5]) == 13);
    REQUIRE(get<int64_t>(output[6]) == 11);
    REQUIRE(get<int64_t>(output[7]) == 12);
    REQUIRE(get<int64_t>(output[8]) == 13);
    REQUIRE(get<int64_t>(output[9]) == 10);
    REQUIRE(get<int64_t>(output[10]) == 11);
    REQUIRE(get<int64_t>(output[11]) == 13);
    REQUIRE(get<int64_t>(output[12]) == 11);
    REQUIRE(get<int64_t>(output[13]) == 12);
    REQUIRE(get<int64_t>(output[14]) == 13);
    REQUIRE(get<int64_t>(output[15]) == 10);
    REQUIRE(get<int64_t>(output[16]) == 11);
    REQUIRE(get<int64_t>(output[17]) == 13);
}

TEST_CASE("Interpreter conditional jumps int", "[script]") {
    auto code = std::vector<byte>{
        push_int, byte(2), push_int, byte(3),
        compare_int, jump_eq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_int, byte(2), push_int, byte(2),
        compare_int, jump_eq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_int, byte(2), push_int, byte(3),
        compare_int, jump_neq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_int, byte(2), push_int, byte(2),
        compare_int, jump_neq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_int, byte(2), push_int, byte(3),
        compare_int, jump_lt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_int, byte(2), push_int, byte(2),
        compare_int, jump_lt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_int, byte(3), push_int, byte(2),
        compare_int, jump_le, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_int, byte(2), push_int, byte(2),
        compare_int, jump_le, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_int, byte(3), push_int, byte(2),
        compare_int, jump_gt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_int, byte(2), push_int, byte(2),
        compare_int, jump_gt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_int, byte(2), push_int, byte(3),
        compare_int, jump_ge, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_int, byte(2), push_int, byte(2),
        compare_int, jump_ge, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 18);
    REQUIRE(get<int64_t>(output[0]) == 10);
    REQUIRE(get<int64_t>(output[1]) == 11);
    REQUIRE(get<int64_t>(output[2]) == 13);
    REQUIRE(get<int64_t>(output[3]) == 11);
    REQUIRE(get<int64_t>(output[4]) == 12);
    REQUIRE(get<int64_t>(output[5]) == 13);
    REQUIRE(get<int64_t>(output[6]) == 11);
    REQUIRE(get<int64_t>(output[7]) == 12);
    REQUIRE(get<int64_t>(output[8]) == 13);
    REQUIRE(get<int64_t>(output[9]) == 10);
    REQUIRE(get<int64_t>(output[10]) == 11);
    REQUIRE(get<int64_t>(output[11]) == 13);
    REQUIRE(get<int64_t>(output[12]) == 11);
    REQUIRE(get<int64_t>(output[13]) == 12);
    REQUIRE(get<int64_t>(output[14]) == 13);
    REQUIRE(get<int64_t>(output[15]) == 10);
    REQUIRE(get<int64_t>(output[16]) == 11);
    REQUIRE(get<int64_t>(output[17]) == 13);
}

TEST_CASE("Interpreter conditional jumps uint", "[script]") {
    auto code = std::vector<byte>{
        push_uint, byte(2), push_uint, byte(3),
        compare_uint, jump_eq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_uint, byte(2), push_uint, byte(2),
        compare_uint, jump_eq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_uint, byte(2), push_uint, byte(3),
        compare_uint, jump_neq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_uint, byte(2), push_uint, byte(2),
        compare_uint, jump_neq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_uint, byte(2), push_uint, byte(3),
        compare_uint, jump_lt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_uint, byte(2), push_uint, byte(2),
        compare_uint, jump_lt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_uint, byte(3), push_uint, byte(2),
        compare_uint, jump_le, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_uint, byte(2), push_uint, byte(2),
        compare_uint, jump_le, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_uint, byte(3), push_uint, byte(2),
        compare_uint, jump_gt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_uint, byte(2), push_uint, byte(2),
        compare_uint, jump_gt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_uint, byte(2), push_uint, byte(3),
        compare_uint, jump_ge, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_uint, byte(2), push_uint, byte(2),
        compare_uint, jump_ge, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 18);
    REQUIRE(get<int64_t>(output[0]) == 10);
    REQUIRE(get<int64_t>(output[1]) == 11);
    REQUIRE(get<int64_t>(output[2]) == 13);
    REQUIRE(get<int64_t>(output[3]) == 11);
    REQUIRE(get<int64_t>(output[4]) == 12);
    REQUIRE(get<int64_t>(output[5]) == 13);
    REQUIRE(get<int64_t>(output[6]) == 11);
    REQUIRE(get<int64_t>(output[7]) == 12);
    REQUIRE(get<int64_t>(output[8]) == 13);
    REQUIRE(get<int64_t>(output[9]) == 10);
    REQUIRE(get<int64_t>(output[10]) == 11);
    REQUIRE(get<int64_t>(output[11]) == 13);
    REQUIRE(get<int64_t>(output[12]) == 11);
    REQUIRE(get<int64_t>(output[13]) == 12);
    REQUIRE(get<int64_t>(output[14]) == 13);
    REQUIRE(get<int64_t>(output[15]) == 10);
    REQUIRE(get<int64_t>(output[16]) == 11);
    REQUIRE(get<int64_t>(output[17]) == 13);
}

#define D2 param_byte8, byte(0), byte(0), byte(0), byte(0), byte(0), byte(0), \
    byte(0), byte(0x40)
#define D3 param_byte8, byte(0), byte(0), byte(0), byte(0), byte(0), byte(0), \
    byte(0x08), byte(0x40)

TEST_CASE("Interpreter conditional jumps double", "[script]") {
    auto code = std::vector<byte>{
        push_double, D2, push_double, D3,
        compare_double, jump_eq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_double, D2, push_double, D2,
        compare_double, jump_eq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_double, D2, push_double, D3,
        compare_double, jump_neq, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_double, D2, push_double, D2,
        compare_double, jump_neq, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_double, D2, push_double, D3,
        compare_double, jump_lt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_double, D2, push_double, D2,
        compare_double, jump_lt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_double, D3, push_double, D2,
        compare_double, jump_le, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_double, D2, push_double, D2,
        compare_double, jump_le, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_double, D3, push_double, D2,
        compare_double, jump_gt, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_double, D2, push_double, D2,
        compare_double, jump_gt, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),

        push_double, D2, push_double, D3,
        compare_double, jump_ge, byte(5),
            push_int, byte(10), test_int, pop, byte(1),
            push_int, byte(11), test_int, pop, byte(1),
        push_double, D2, push_double, D2,
        compare_double, jump_ge, byte(5),
            push_int, byte(12), test_int, pop, byte(1),
            push_int, byte(13), test_int, pop, byte(1),
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 18);
    REQUIRE(get<int64_t>(output[0]) == 10);
    REQUIRE(get<int64_t>(output[1]) == 11);
    REQUIRE(get<int64_t>(output[2]) == 13);
    REQUIRE(get<int64_t>(output[3]) == 11);
    REQUIRE(get<int64_t>(output[4]) == 12);
    REQUIRE(get<int64_t>(output[5]) == 13);
    REQUIRE(get<int64_t>(output[6]) == 11);
    REQUIRE(get<int64_t>(output[7]) == 12);
    REQUIRE(get<int64_t>(output[8]) == 13);
    REQUIRE(get<int64_t>(output[9]) == 10);
    REQUIRE(get<int64_t>(output[10]) == 11);
    REQUIRE(get<int64_t>(output[11]) == 13);
    REQUIRE(get<int64_t>(output[12]) == 11);
    REQUIRE(get<int64_t>(output[13]) == 12);
    REQUIRE(get<int64_t>(output[14]) == 13);
    REQUIRE(get<int64_t>(output[15]) == 10);
    REQUIRE(get<int64_t>(output[16]) == 11);
    REQUIRE(get<int64_t>(output[17]) == 13);
}
