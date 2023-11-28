#include <catch2/catch_test_macros.hpp>

#include "ganim/object/value.hpp"

#include "ganim/animation/animation.hpp"
#include "test/ganim/ga_equals.hpp"

using namespace ganim;

TEST_CASE("Value tracker double", "[object]") {
    auto val = Value();
    REQUIRE(val.get_value() == 0);
    REQUIRE(val() == 0);
    REQUIRE(val == 0);
    val.set_value(1.5);
    REQUIRE(val.get_value() == 1.5);
    REQUIRE(val() == 1.5);
    REQUIRE(val == 1.5);
    val = Value(0.5);
    REQUIRE(val.get_value() == 0.5);
    REQUIRE(val() == 0.5);
    REQUIRE(val == 0.5);
    val = 2.5;
    REQUIRE(val.get_value() == 2.5);
    REQUIRE(val() == 2.5);
    REQUIRE(val == 2.5);

    val.set_fps(4);
    animate(val, {.rate_function = [](double t){return t;}}) = 3.5;
    REQUIRE(val.get_value() == 2.5);
    REQUIRE(val() == 2.5);
    REQUIRE(val == 2.5);
    val.update();
    REQUIRE(val.get_value() == 2.75);
    REQUIRE(val() == 2.75);
    REQUIRE(val == 2.75);
    val.update();
    REQUIRE(val.get_value() == 3);
    REQUIRE(val() == 3);
    REQUIRE(val == 3);
    val.update();
    REQUIRE(val.get_value() == 3.25);
    REQUIRE(val() == 3.25);
    REQUIRE(val == 3.25);
    val.update();
    REQUIRE(val.get_value() == 3.5);
    REQUIRE(val() == 3.5);
    REQUIRE(val == 3.5);
    val.update();
    REQUIRE(val.get_value() == 3.5);
    REQUIRE(val() == 3.5);
    REQUIRE(val == 3.5);
}

TEST_CASE("Value tracker vector", "[object]") {
    using namespace vga2;
    auto val = Value<Vector>();
    REQUIRE(val.get_value() == 0);
    REQUIRE(val() == 0);
    REQUIRE(val == 0);
    val.set_value(e1);
    REQUIRE_THAT(val.get_value(), GAEquals(e1));
    REQUIRE_THAT(val(), GAEquals(e1));
    val = Value(e2);
    REQUIRE_THAT(val.get_value(), GAEquals(e2));
    REQUIRE_THAT(val(), GAEquals(e2));
    val = e1 + e2;
    REQUIRE_THAT(val.get_value(), GAEquals(e1 + e2));
    REQUIRE_THAT(val(), GAEquals(e1 + e2));

    val.set_fps(4);
    animate(val, {.rate_function = [](double t){return t;}}) = 2*e1 + e2;
    REQUIRE_THAT(val.get_value(), GAEquals(e1 + e2));
    REQUIRE_THAT(val(), GAEquals(e1 + e2));
    val.update();
    REQUIRE_THAT(val.get_value(), GAEquals(1.25*e1 + e2));
    REQUIRE_THAT(val(), GAEquals(1.25*e1 + e2));
    val.update();
    REQUIRE_THAT(val.get_value(), GAEquals(1.5*e1 + e2));
    REQUIRE_THAT(val(), GAEquals(1.5*e1 + e2));
    val.update();
    REQUIRE_THAT(val.get_value(), GAEquals(1.75*e1 + e2));
    REQUIRE_THAT(val(), GAEquals(1.75*e1 + e2));
    val.update();
    REQUIRE_THAT(val.get_value(), GAEquals(2*e1 + e2));
    REQUIRE_THAT(val(), GAEquals(2*e1 + e2));
    val.update();
    REQUIRE_THAT(val.get_value(), GAEquals(2*e1 + e2));
    REQUIRE_THAT(val(), GAEquals(2*e1 + e2));
}

namespace {
    class NotZeroConstructible {};
    auto value = Value<NotZeroConstructible>(); // Check if this will compile
}
