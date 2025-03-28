#include <catch2/catch_test_macros.hpp>

#include "script/expression/constant.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("StringConstant") {
    auto expr = Constant(1, 2, 3);
    REQUIRE(expr.type() == any_pointer::get_tag<int>());
    REQUIRE(expr.value());
    REQUIRE(expr.value().get_as<int>());
    REQUIRE(*expr.value().get_as<int>() == 1);
    REQUIRE(expr.line_number() == 2);
    REQUIRE(expr.column_number() == 3);
}
