#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter call", "[script]") {
    auto code = std::vector<byte>{
        push_int, byte(1),
        call_medium, byte(15), byte(0),
        push_int, byte(2),
        call_medium, byte(10), byte(0),
        push_int, byte(3),
        call_param, byte(20),
        push_int, byte(4),
        call_param, byte(20),
        jump_short, byte(4),
        test_int, pop, byte(1), ret
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 4);
    REQUIRE(get<int64_t>(output[0]) == 1);
    REQUIRE(get<int64_t>(output[1]) == 2);
    REQUIRE(get<int64_t>(output[2]) == 3);
    REQUIRE(get<int64_t>(output[3]) == 4);
}

TEST_CASE("Interpreter call recursion", "[script]") {
    // void f(int i)
    // {
    //     output(i)
    //     if (i != 5) {
    //          f(i + 1)
    //     }
    //     output(i)
    // }
    // f(0)
    auto code = std::vector<byte>{
        push_int, byte(0),
        call_medium, byte(2), byte(0),
        jump_short, byte(20),
        // Definition of f
        // output(i)
        test_int,
        // if (i != 5)
        push_int, param_stack1,
        push_int, byte(5),
        compare_int, jump_eq, byte(8),
        // f(i + 1)
        push_int, param_stack1, push_int, byte(1), plus_int,
        call_medium, byte(0xF0), byte(0xFF),
        // output(i)
        test_int, pop, byte(1),
        // End of f
        ret
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 12);
    REQUIRE(get<int64_t>(output[0]) == 0);
    REQUIRE(get<int64_t>(output[1]) == 1);
    REQUIRE(get<int64_t>(output[2]) == 2);
    REQUIRE(get<int64_t>(output[3]) == 3);
    REQUIRE(get<int64_t>(output[4]) == 4);
    REQUIRE(get<int64_t>(output[5]) == 5);
    REQUIRE(get<int64_t>(output[6]) == 5);
    REQUIRE(get<int64_t>(output[7]) == 4);
    REQUIRE(get<int64_t>(output[8]) == 3);
    REQUIRE(get<int64_t>(output[9]) == 2);
    REQUIRE(get<int64_t>(output[10]) == 1);
    REQUIRE(get<int64_t>(output[11]) == 0);
}
