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
        std::byte(200)
    };
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 2);
    REQUIRE(get<std::byte>(output[0]) == std::byte(42));
    REQUIRE(get<std::byte>(output[1]) == std::byte(200));
}
