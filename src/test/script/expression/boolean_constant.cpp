#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/expression/boolean_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("BooleanConstant") {
    auto expr = BooleanConstant(true, 2, 3);
    REQUIRE(expr.type() == TypeID::Boolean);
    REQUIRE_THROWS_WITH(expr.as_string(),
            get_runtime_error_message(2, 3,
                "Boolean constant true is not a string."));
    REQUIRE_THROWS_WITH(expr.as_integer(),
            get_runtime_error_message(2, 3,
                "Boolean constant true is not an integer."));
}
