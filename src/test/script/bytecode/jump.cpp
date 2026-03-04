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
    // 2 4 6 3 7 5
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
