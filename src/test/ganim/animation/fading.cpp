#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/fading.hpp"
#include "ganim/object/bases/object.hpp"

using namespace ganim;

TEST_CASE("Fading", "[animation]") {
    auto test = Object();
    test.set_fps(4);
    fade_in(test);
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x0);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xBF);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xFF);
    fade_out(test);
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xFF);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xBF);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0xFF);

    test.set_opacity(0.5);
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    fade_in(test, {.duration = 0.5});
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x0);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    fade_out(test, {.duration = 0.5});
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
}
