#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>

#include "ganim/rate_functions.hpp"

using namespace ganim::rf;
using namespace Catch::Matchers;

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

TEST_CASE("Smootherstep rate function", "[ganim]") {
    REQUIRE(smootherstep(0) == 0);
    REQUIRE(smootherstep(0.25) == 0.103515625);
    REQUIRE(smootherstep(0.5) == 0.5);
    REQUIRE(smootherstep(0.75) == 0.896484375);
    REQUIRE(smootherstep(1) == 1);
}

TEST_CASE("Smoothererstep rate function", "[ganim]") {
    REQUIRE(smoothererstep(0) == 0);
    REQUIRE(smoothererstep(0.25) == 0.070556640625);
    REQUIRE(smoothererstep(0.5) == 0.5);
    REQUIRE(smoothererstep(0.75) == 0.929443359375);
    REQUIRE(smoothererstep(1) == 1);
}

TEST_CASE("Cosine rate function", "[ganim]") {
    REQUIRE_THAT(cosine(0), WithinAbs(0, 1e-5));
    REQUIRE_THAT(cosine(0.25), WithinAbs(0.5 - std::sqrt(2)/4, 1e-5));
    REQUIRE_THAT(cosine(0.5), WithinAbs(0.5, 1e-5));
    REQUIRE_THAT(cosine(0.75), WithinAbs(0.5 + std::sqrt(2)/4, 1e-5));
    REQUIRE_THAT(cosine(1), WithinAbs(1, 1e-5));
}
