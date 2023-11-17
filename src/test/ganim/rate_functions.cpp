#include <catch2/catch_test_macros.hpp>

#include "ganim/rate_functions.hpp"

using namespace ganim::rf;

TEST_CASE("Linear rate function", "[ganim]") {
    REQUIRE(linear(0) == 0);
    REQUIRE(linear(0.25) == 0.25);
    REQUIRE(linear(0.5) == 0.5);
    REQUIRE(linear(0.75) == 0.75);
    REQUIRE(linear(1) == 1);
}

TEST_CASE("Smoothstep rate function", "[ganim]") {
    REQUIRE(smoothstep(0) == 0);
    REQUIRE(smoothstep(0.25) == 0.15625);
    REQUIRE(smoothstep(0.5) == 0.5);
    REQUIRE(smoothstep(0.75) == 0.84375);
    REQUIRE(smoothstep(1) == 1);
}
