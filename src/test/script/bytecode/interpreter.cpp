#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter constant parameters", "[script]") {
    auto code = std::vector<std::byte>{
        push_byte,
        std::byte(42),
        test_byte,
        push_byte,
        std::byte(0b10000000),
        std::byte(200),
        test_byte,

        push_int,
        std::byte(42),
        test_int,
        push_int,
        std::byte(0b10000000),
        std::byte(0xFF),
        test_int,
        push_int,
        std::byte(0b10000001),
        std::byte(0x00),
        std::byte(0x02),
        test_int,
        push_int,
        std::byte(0b10000001),
        std::byte(0xFE),
        std::byte(0xFF),
        test_int,
        push_int,
        std::byte(0b10000010),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x02),
        test_int,
        push_int,
        std::byte(0b10000010),
        std::byte(0xFD),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        test_int,
        push_int,
        std::byte(0b10000011),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x02),
        test_int,
        push_int,
        std::byte(0b10000011),
        std::byte(0xFC),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        test_int,

        push_uint,
        std::byte(42),
        test_uint,
        push_uint,
        std::byte(0b10000000),
        std::byte(0xFF),
        test_uint,
        push_uint,
        std::byte(0b10000001),
        std::byte(0x00),
        std::byte(0x02),
        test_uint,
        push_uint,
        std::byte(0b10000001),
        std::byte(0xFE),
        std::byte(0xFF),
        test_uint,
        push_uint,
        std::byte(0b10000010),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x02),
        test_uint,
        push_uint,
        std::byte(0b10000010),
        std::byte(0xFD),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        test_uint,
        push_uint,
        std::byte(0b10000011),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x02),
        test_uint,
        push_uint,
        std::byte(0b10000011),
        std::byte(0xFC),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        test_uint,

        push_double,
        std::byte(0b10000011),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0xE0),
        std::byte(0xBF),
        test_double,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 19);

    REQUIRE(get<std::byte>(output[0]) == std::byte(42));
    REQUIRE(get<std::byte>(output[1]) == std::byte(200));

    REQUIRE(get<std::int64_t>(output[2]) == 42LL);
    REQUIRE(get<std::int64_t>(output[3]) == -1LL);
    REQUIRE(get<std::int64_t>(output[4]) == 0x200LL);
    REQUIRE(get<std::int64_t>(output[5]) == -2LL);
    REQUIRE(get<std::int64_t>(output[6]) == 0x2000000LL);
    REQUIRE(get<std::int64_t>(output[7]) == -3LL);
    REQUIRE(get<std::int64_t>(output[8]) == 0x200000000000000LL);
    REQUIRE(get<std::int64_t>(output[9]) == -4LL);

    REQUIRE(get<std::uint64_t>(output[10]) == 42ULL);
    REQUIRE(get<std::uint64_t>(output[11]) == 0xFFULL);
    REQUIRE(get<std::uint64_t>(output[12]) == 0x200ULL);
    REQUIRE(get<std::uint64_t>(output[13]) == 0xFFFEULL);
    REQUIRE(get<std::uint64_t>(output[14]) == 0x2000000ULL);
    REQUIRE(get<std::uint64_t>(output[15]) == 0xFFFFFFFDULL);
    REQUIRE(get<std::uint64_t>(output[16]) == 0x200000000000000ULL);
    REQUIRE(get<std::uint64_t>(output[17]) == 0xFFFFFFFFFFFFFFFCULL);

    REQUIRE(get<double>(output[18]) == -0.5);
}

TEST_CASE("Interpreter push_pop", "[script]") {
    auto code = std::vector<std::byte>{
        push_int,
        std::byte(42),
        test_int,
        push_int,
        std::byte(43),
        test_int,
        pop,
        std::byte(1),
        test_int,
        push_byte,
        std::byte(44),
        test_byte,
        push_byte,
        std::byte(45),
        push_int,
        std::byte(1),
        pop,
        std::byte(1),
        test_byte,
        pop,
        std::byte(2),
        test_int,
        pop,
        std::byte(1),

        push_uint,
        std::byte(46),
        test_uint,
        push_double,
        std::byte(0b10000011),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0xE0),
        std::byte(0xBF),
        test_double,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 8);

    REQUIRE(get<std::int64_t>(output[0]) == 42);
    REQUIRE(get<std::int64_t>(output[1]) == 43);
    REQUIRE(get<std::int64_t>(output[2]) == 42);
    REQUIRE(get<std::byte>(output[3]) == std::byte(44));
    REQUIRE(get<std::byte>(output[4]) == std::byte(45));
    REQUIRE(get<std::int64_t>(output[5]) == 42);
    REQUIRE(get<std::uint64_t>(output[6]) == 46);
    REQUIRE(get<double>(output[7]) == -0.5);
}
