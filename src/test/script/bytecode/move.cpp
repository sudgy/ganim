#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter moving onto the stack", "[script]") {
    auto code = std::vector<byte>{
        push_int, byte(1),
        push_int, byte(2),
        push_int, byte(3),
        push_int, byte(4),
        move_stack, byte(1),
        move_stack, byte(0),
        test_int,
        pop, byte(1),
        test_int
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 2);
    REQUIRE(get<int64_t>(output[0]) == 4);
    REQUIRE(get<int64_t>(output[1]) == 3);
}

TEST_CASE("Interpreter moving globally", "[script]") {
    auto code = std::vector<byte>{
        push_int, byte(1),
        push_int, byte(2),
        enter, byte(0),
        push_int, byte(3),
        move_global, byte(1),
        test_int,
        pop, byte(1),
        test_int
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 2);
    REQUIRE(get<int64_t>(output[0]) == 3);
    REQUIRE(get<int64_t>(output[1]) == 1);
}

TEST_CASE("Interpreter moving between two stack values", "[script]") {
    auto code = std::vector<byte>{
        push_int, byte(1),
        push_int, byte(2),
        push_int, byte(3),
        move_stack2, byte(1), byte(0),
        test_int,
        pop, byte(1),
        test_int,
        pop, byte(1),
        test_int
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 3);
    REQUIRE(get<int64_t>(output[0]) == 3);
    REQUIRE(get<int64_t>(output[1]) == 1);
    REQUIRE(get<int64_t>(output[2]) == 1);
}
