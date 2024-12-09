#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/fading.hpp"
#include "ganim/object/bases/object.hpp"

#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

namespace {
    class TestObject : public Object {
        virtual Box get_original_true_bounding_box() const override
            {return Box();}
        virtual void draw(const Camera&) override {}
        virtual void draw_outline(const Camera&) override {}
        virtual void set_outline(const Color&, double) override {}
        virtual void invalidate_outline() override {}
        virtual Color get_outline_color() const override {return Color();}
        virtual double get_outline_thickness() const override {return 0.0;}
    };
}

TEST_CASE("Fading", "[animation]") {
    auto test = TestObject();
    auto scene = TestScene(1, 1, 1, 1, 4);
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

TEST_CASE("Fading multiple objects", "[animation]") {
    auto test1 = TestObject();
    auto test2 = TestObject();
    auto scene = TestScene(1, 1, 1, 1, 2);
    scene.add(test1, test2);
    // I can't think of a simple way to test that this actually affects an
    // rvalue, but honestly as long as it compiles and runs without crashing
    // it's probably fine
    fade_in(scene, test1, test2, TestObject());

    REQUIRE(test1.is_visible());
    REQUIRE(test2.is_visible());
    REQUIRE(test1.get_color().a == 0x0);
    REQUIRE(test2.get_color().a == 0x0);
    scene.frame_advance();
    REQUIRE(test1.is_visible());
    REQUIRE(test2.is_visible());
    REQUIRE(test1.get_color().a == 0x7F);
    REQUIRE(test2.get_color().a == 0x7F);
    scene.frame_advance();
    REQUIRE(test1.is_visible());
    REQUIRE(test2.is_visible());
    REQUIRE(test1.get_color().a == 0xFF);
    REQUIRE(test2.get_color().a == 0xFF);

    fade_out(scene, test1, test2, TestObject());
    REQUIRE(test1.is_visible());
    REQUIRE(test2.is_visible());
    REQUIRE(test1.get_color().a == 0xFF);
    REQUIRE(test2.get_color().a == 0xFF);
    scene.frame_advance();
    REQUIRE(test1.is_visible());
    REQUIRE(test2.is_visible());
    REQUIRE(test1.get_color().a == 0x7F);
    REQUIRE(test2.get_color().a == 0x7F);
    scene.frame_advance();
    REQUIRE(!test1.is_visible());
    REQUIRE(!test2.is_visible());
    REQUIRE(test1.get_color().a == 0xFF);
    REQUIRE(test2.get_color().a == 0xFF);
}
