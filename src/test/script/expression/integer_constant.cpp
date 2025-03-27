#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/expression/integer_constant.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("IntegerConstant") {
    auto expr = IntegerConstant(1, 2, 3);
    REQUIRE(expr.type() == TypeID::Integer);
    REQUIRE_THROWS_WITH(expr.as_string(),
            get_runtime_error_message(2, 3,
                "Integer constant 1 is not a string."));
}
