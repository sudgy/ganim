#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter parameters", "[script]") {
    auto code = std::vector<std::byte>{
        test_output_byte,
        std::byte(42),
        test_output_byte,
        std::byte(0b10000000),
        std::byte(200),

        test_output_int,
        std::byte(42),
        test_output_int,
        std::byte(0b10000000),
        std::byte(0xFF),
        test_output_int,
        std::byte(0b10000001),
        std::byte(0x00),
        std::byte(0x02),
        test_output_int,
        std::byte(0b10000001),
        std::byte(0xFE),
        std::byte(0xFF),
        test_output_int,
        std::byte(0b10000010),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x02),
        test_output_int,
        std::byte(0b10000010),
        std::byte(0xFD),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        test_output_int,
        std::byte(0b10000011),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x00),
        std::byte(0x02),
        test_output_int,
        std::byte(0b10000011),
        std::byte(0xFC),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
        std::byte(0xFF),
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 10);

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
}
