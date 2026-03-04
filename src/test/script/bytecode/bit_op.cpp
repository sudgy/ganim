#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter bool ops", "[script]") {
    auto code = std::vector<byte>{
        push_byte, byte(0), push_byte, byte(0), and_byte, test_byte,
        push_byte, byte(0), push_byte, byte(1), and_byte, test_byte,
        push_byte, byte(1), push_byte, byte(0), and_byte, test_byte,
        push_byte, byte(1), push_byte, byte(1), and_byte, test_byte,

        push_byte, byte(0), push_byte, byte(0), or_byte, test_byte,
        push_byte, byte(0), push_byte, byte(1), or_byte, test_byte,
        push_byte, byte(1), push_byte, byte(0), or_byte, test_byte,
        push_byte, byte(1), push_byte, byte(1), or_byte, test_byte,

        push_byte, byte(0), push_byte, byte(0), xor_byte, test_byte,
        push_byte, byte(0), push_byte, byte(1), xor_byte, test_byte,
        push_byte, byte(1), push_byte, byte(0), xor_byte, test_byte,
        push_byte, byte(1), push_byte, byte(1), xor_byte, test_byte,

        push_byte, byte(0), not_bool, test_byte,
        push_byte, byte(1), not_bool, test_byte,
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 14);
    REQUIRE(get<byte>(output[0]) == byte(0));
    REQUIRE(get<byte>(output[1]) == byte(0));
    REQUIRE(get<byte>(output[2]) == byte(0));
    REQUIRE(get<byte>(output[3]) == byte(1));
    REQUIRE(get<byte>(output[4]) == byte(0));
    REQUIRE(get<byte>(output[5]) == byte(1));
    REQUIRE(get<byte>(output[6]) == byte(1));
    REQUIRE(get<byte>(output[7]) == byte(1));
    REQUIRE(get<byte>(output[8]) == byte(0));
    REQUIRE(get<byte>(output[9]) == byte(1));
    REQUIRE(get<byte>(output[10]) == byte(1));
    REQUIRE(get<byte>(output[11]) == byte(0));
    REQUIRE(get<byte>(output[12]) == byte(1));
    REQUIRE(get<byte>(output[13]) == byte(0));
}
