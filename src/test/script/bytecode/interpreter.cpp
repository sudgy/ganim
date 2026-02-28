#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter constant parameters", "[script]") {
    auto code = std::vector<byte>{
        push_byte,
        byte(42),
        test_byte,
        push_byte,
        byte(0b10000000),
        byte(200),
        test_byte,

        push_int,
        byte(42),
        test_int,
        push_int,
        byte(0b10000000),
        byte(0xFF),
        test_int,
        push_int,
        byte(0b10000001),
        byte(0x00),
        byte(0x02),
        test_int,
        push_int,
        byte(0b10000001),
        byte(0xFE),
        byte(0xFF),
        test_int,
        push_int,
        byte(0b10000010),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x02),
        test_int,
        push_int,
        byte(0b10000010),
        byte(0xFD),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        test_int,
        push_int,
        byte(0b10000011),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x02),
        test_int,
        push_int,
        byte(0b10000011),
        byte(0xFC),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        test_int,

        push_uint,
        byte(42),
        test_uint,
        push_uint,
        byte(0b10000000),
        byte(0xFF),
        test_uint,
        push_uint,
        byte(0b10000001),
        byte(0x00),
        byte(0x02),
        test_uint,
        push_uint,
        byte(0b10000001),
        byte(0xFE),
        byte(0xFF),
        test_uint,
        push_uint,
        byte(0b10000010),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x02),
        test_uint,
        push_uint,
        byte(0b10000010),
        byte(0xFD),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        test_uint,
        push_uint,
        byte(0b10000011),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x02),
        test_uint,
        push_uint,
        byte(0b10000011),
        byte(0xFC),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        byte(0xFF),
        test_uint,

        push_double,
        byte(0b10000011),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0xE0),
        byte(0xBF),
        test_double,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 19);

    REQUIRE(get<byte>(output[0]) == byte(42));
    REQUIRE(get<byte>(output[1]) == byte(200));

    REQUIRE(get<int64_t>(output[2]) == 42LL);
    REQUIRE(get<int64_t>(output[3]) == -1LL);
    REQUIRE(get<int64_t>(output[4]) == 0x200LL);
    REQUIRE(get<int64_t>(output[5]) == -2LL);
    REQUIRE(get<int64_t>(output[6]) == 0x2000000LL);
    REQUIRE(get<int64_t>(output[7]) == -3LL);
    REQUIRE(get<int64_t>(output[8]) == 0x200000000000000LL);
    REQUIRE(get<int64_t>(output[9]) == -4LL);

    REQUIRE(get<uint64_t>(output[10]) == 42ULL);
    REQUIRE(get<uint64_t>(output[11]) == 0xFFULL);
    REQUIRE(get<uint64_t>(output[12]) == 0x200ULL);
    REQUIRE(get<uint64_t>(output[13]) == 0xFFFEULL);
    REQUIRE(get<uint64_t>(output[14]) == 0x2000000ULL);
    REQUIRE(get<uint64_t>(output[15]) == 0xFFFFFFFDULL);
    REQUIRE(get<uint64_t>(output[16]) == 0x200000000000000ULL);
    REQUIRE(get<uint64_t>(output[17]) == 0xFFFFFFFFFFFFFFFCULL);

    REQUIRE(get<double>(output[18]) == -0.5);
}

TEST_CASE("Interpreter push_pop", "[script]") {
    auto code = std::vector<byte>{
        push_int,
        byte(42),
        test_int,
        push_int,
        byte(43),
        test_int,
        pop,
        byte(1),
        test_int,
        push_byte,
        byte(44),
        test_byte,
        push_byte,
        byte(45),
        push_int,
        byte(1),
        pop,
        byte(1),
        test_byte,
        pop,
        byte(2),
        test_int,
        pop,
        byte(1),

        push_uint,
        byte(46),
        test_uint,
        push_double,
        byte(0b10000011),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0x00),
        byte(0xE0),
        byte(0xBF),
        test_double,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 8);

    REQUIRE(get<int64_t>(output[0]) == 42);
    REQUIRE(get<int64_t>(output[1]) == 43);
    REQUIRE(get<int64_t>(output[2]) == 42);
    REQUIRE(get<byte>(output[3]) == byte(44));
    REQUIRE(get<byte>(output[4]) == byte(45));
    REQUIRE(get<int64_t>(output[5]) == 42);
    REQUIRE(get<uint64_t>(output[6]) == 46);
    REQUIRE(get<double>(output[7]) == -0.5);
}

TEST_CASE("Interpreter stack frame parameters", "[script]") {
    auto code = std::vector<byte>{
        push_int,
        byte(12),
        push_uint,
        byte(13),
        push_byte,
        byte(14),
        push_int,
        param_stack_frame,
        byte(0),
        test_int,
        push_uint,
        param_stack_frame,
        byte(1),
        test_uint,
        push_byte,
        param_stack_frame,
        byte(2),
        test_byte,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 3);

    REQUIRE(get<int64_t>(output[0]) == 12);
    REQUIRE(get<uint64_t>(output[1]) == 13);
    REQUIRE(get<byte>(output[2]) == byte(14));
}
