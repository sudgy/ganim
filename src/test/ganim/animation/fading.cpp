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
        virtual void set_outline(const Color&, double, bool) override {}
        virtual void invalidate_outline() override {}
        virtual Color get_outline_color() const override {return Color();}
        virtual double get_outline_thickness() const override {return 0.0;}
    };
}

TEST_CASE("Fading", "[animation]") {
    auto test = ObjectPtr<TestObject>();
    auto scene = TestScene(1, 1, 1, 1, 4);
    test->set_fps(4);
    fade_in(scene, test);
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.25);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.5);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.75);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 1);
    fade_out(scene, test);
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 1);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.75);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.5);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.25);
    test->update();
    REQUIRE(!test->is_visible());
    REQUIRE(test->get_opacity() == 1);

    test->set_opacity(0.5);
    REQUIRE(!test->is_visible());
    REQUIRE(test->get_opacity() == 0.5);
    fade_in(scene, test, {.duration = 0.5});
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.5);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 1.0);
    fade_out(scene, test, {.duration = 0.5});
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 1.0);
    test->update();
    REQUIRE(test->is_visible());
    REQUIRE(test->get_opacity() == 0.5);
    test->update();
    REQUIRE(!test->is_visible());
    REQUIRE(test->get_opacity() == 1.0);
}

TEST_CASE("Fading multiple objects", "[animation]") {
    auto test1 = ObjectPtr<TestObject>();
    auto test2 = ObjectPtr<TestObject>();
    auto scene = TestScene(1, 1, 1, 1, 2);
    scene.add(test1, test2);
    // I can't think of a simple way to test that this actually affects an
    // rvalue, but honestly as long as it compiles and runs without crashing
    // it's probably fine
    fade_in(scene, test1, test2, ObjectPtr<TestObject>());

    REQUIRE(test1->is_visible());
    REQUIRE(test2->is_visible());
    REQUIRE(test1->get_opacity() == 0);
    REQUIRE(test2->get_opacity() == 0);
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test2->is_visible());
    REQUIRE(test1->get_opacity() == 0.5);
    REQUIRE(test2->get_opacity() == 0.5);
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test2->is_visible());
    REQUIRE(test1->get_opacity() == 1);
    REQUIRE(test2->get_opacity() == 1);

    fade_out(scene, test1, test2, ObjectPtr<TestObject>());
    REQUIRE(test1->is_visible());
    REQUIRE(test2->is_visible());
    REQUIRE(test1->get_opacity() == 1);
    REQUIRE(test2->get_opacity() == 1);
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test2->is_visible());
    REQUIRE(test1->get_opacity() == 0.5);
    REQUIRE(test2->get_opacity() == 0.5);
    scene.frame_advance();
    REQUIRE(!test1->is_visible());
    REQUIRE(!test2->is_visible());
    REQUIRE(test1->get_opacity() == 1);
    REQUIRE(test2->get_opacity() == 1);
}
