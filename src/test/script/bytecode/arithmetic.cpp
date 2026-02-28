#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter unary functions", "[script]") {
    auto code = std::vector<byte>{
        push_int,
        byte(5),
        unary_minus_int,
        test_int,
        pop,
        byte(1),

        push_double,
        param_byte8,
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        unary_minus_double,
        test_double,
        pop,
        byte(1),
    };
    *reinterpret_cast<double*>(&code[8]) = 4.5;
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 2);

    REQUIRE(get<int64_t>(output[0]) == -5);
    REQUIRE(get<double>(output[1]) == -4.5);
}
