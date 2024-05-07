#include <catch2/catch_test_macros.hpp>

#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"

#include "ganim/object/group.hpp"
#include "ganim/object/shape.hpp"

using namespace ganim;

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
