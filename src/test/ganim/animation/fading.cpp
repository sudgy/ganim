#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/fading.hpp"
#include "ganim/object/bases/object.hpp"

#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

namespace {
    class TestObject : public Object {
        virtual Box get_true_bounding_box() const override {return Box();}
    };
}

TEST_CASE("Fading", "[animation]") {
    auto test = TestObject();
    auto scene = TestScene(1, 1, 1, 1, 1);
    test.set_fps(4);
    fade_in(scene, test);
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
    fade_out(scene, test);
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
    fade_in(scene, test, {.duration = 0.5});
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x0);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    fade_out(scene, test, {.duration = 0.5});
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
}
