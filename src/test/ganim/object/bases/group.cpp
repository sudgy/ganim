#include <catch2/catch_test_macros.hpp>

#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"

#include "ganim/object/bases/group.hpp"
#include "ganim/object/shape.hpp"
#include "ganim/animation/animation.hpp"

using namespace ganim;

namespace {
    class TestObject : public SingleObject {
        public:
            virtual void draw(const Camera&) override {}
            virtual Box get_true_bounding_box() const override {return Box();}
    };
}

TEST_CASE("Group drawing", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 4);
    auto shape1 = Shape(
        {{ 0,  2, 0, 0, 1, 0, 0, 0.5},
         { 0, -2, 0, 0, 1, 0, 0, 0.5},
         {-2, -2, 0, 0, 1, 0, 0, 0.5},
         {-2,  2, 0, 0, 1, 0, 0, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape2 = Shape(
        {{1,  2, 0, 0, 0, 1, 0, 0.5},
         {1, -2, 0, 0, 0, 1, 0, 0.5},
         {-1, -2, 0, 0, 0, 1, 0, 0.5},
         {-1,  2, 0, 0, 0, 1, 0, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape3 = Shape(
        {{2,  2, 0, 0, 0, 0, 1, 0.5},
         {2, -2, 0, 0, 0, 0, 1, 0.5},
         {0, -2, 0, 0, 0, 0, 1, 0.5},
         {0,  2, 0, 0, 0, 0, 1, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto group = Group(shape1, shape2);
    group.set_visible(true);
    shape3.set_visible(true);
    scene.add(group, shape3);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == ApproxColor("800000"));
    REQUIRE(scene.get_pixel(0, 1, 0) == ApproxColor("408000"));
    REQUIRE(scene.get_pixel(0, 2, 0) == ApproxColor("004080"));
    REQUIRE(scene.get_pixel(0, 3, 0) == ApproxColor("000080"));
}

TEST_CASE("Group animating subobjects", "[object]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = TestObject();
    auto test = Group(obj);
    scene.add(test);
    animate(scene, obj); // Verify that the object got its fps set
}
