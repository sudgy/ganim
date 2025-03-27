#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/expression/string_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("StringConstant") {
    auto expr = StringConstant("1", 2, 3);
    REQUIRE(expr.type() == ExpressionType::String);
    REQUIRE_THROWS_WITH(expr.as_integer(),
            get_runtime_error_message(2, 3,
                "String constant \"1\" is not an integer."));
}
