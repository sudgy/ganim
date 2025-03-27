#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/expression/string_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

// I haven't made a way to change what Print outputs to so I won't bother
// writing tests for that until I do that

TEST_CASE("StringConstant error handling") {
    auto expr = StringConstant("1", 2, 3);
    REQUIRE_THROWS_WITH(expr.as_integer(),
            get_runtime_error_message(2, 3,
                "String constant \"1\" is not an integer."));
}
