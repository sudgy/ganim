#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/group_animate.hpp"
#include "ganim/animation/fading.hpp"
#include "ganim/animation/creation.hpp"
#include "ganim/object/bases/group.hpp"

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

TEST_CASE("Group animation", "[animation]") {
    auto test1 = ObjectPtr<TestObject>();
    auto test2 = ObjectPtr<TestObject>();
    auto test = make_group(test1, test2);
    auto scene = TestScene(1, 1, 1, 1, 4);
    scene.add(test);
    auto my_fade_in = [](SceneBase& scene, ObjectPtr<Object> obj) {
        fade_in(scene, obj, {.rate_function = rf::linear});
    };
    group_animate(scene, test, my_fade_in);
    REQUIRE(!test1->is_visible());
    REQUIRE(!test2->is_visible());
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test1->get_opacity() == 0.25);
    REQUIRE(!test2->is_visible());
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test1->get_opacity() == 0.5);
    REQUIRE(!test2->is_visible());
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test1->get_opacity() == 0.75);
    REQUIRE(test2->is_visible());
    REQUIRE(test2->get_opacity() == 0.25);
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test1->get_opacity() == 1.0);
    REQUIRE(test2->is_visible());
    REQUIRE(test2->get_opacity() == 0.5);
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test1->get_opacity() == 1.0);
    REQUIRE(test2->is_visible());
    REQUIRE(test2->get_opacity() == 0.75);
    scene.frame_advance();
    REQUIRE(test1->is_visible());
    REQUIRE(test1->get_opacity() == 1.0);
    REQUIRE(test2->is_visible());
    REQUIRE(test2->get_opacity() == 1.0);
}
