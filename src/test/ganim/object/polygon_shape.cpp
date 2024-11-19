#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/polygon_shape.hpp"

using namespace ganim;

TEST_CASE("PolygonShape", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 1);
    using namespace vga2;
    auto shape = PolygonShape({
        0*e1,
        e1,
        e1 + e2,
        -e1 + e2,
        -e1 - 2*e2,
        2*e1 - 2*e2,
        2*e1 - e2,
        -e2
    });
    scene.add(shape);
    shape.set_visible(true);
    scene.frame_advance();
    const auto white = Color("FFFFFF");
    const auto black = Color("000000");
    REQUIRE(scene.get_pixel(0, 0, 0) == black);
    REQUIRE(scene.get_pixel(0, 1, 0) == black);
    REQUIRE(scene.get_pixel(0, 2, 0) == black);
    REQUIRE(scene.get_pixel(0, 3, 0) == black);
    REQUIRE(scene.get_pixel(0, 0, 1) == black);
    REQUIRE(scene.get_pixel(0, 1, 1) == white);
    REQUIRE(scene.get_pixel(0, 2, 1) == white);
    REQUIRE(scene.get_pixel(0, 3, 1) == black);
    REQUIRE(scene.get_pixel(0, 0, 2) == black);
    REQUIRE(scene.get_pixel(0, 1, 2) == white);
    REQUIRE(scene.get_pixel(0, 2, 2) == black);
    REQUIRE(scene.get_pixel(0, 3, 2) == black);
    REQUIRE(scene.get_pixel(0, 0, 3) == black);
    REQUIRE(scene.get_pixel(0, 1, 3) == white);
    REQUIRE(scene.get_pixel(0, 2, 3) == white);
    REQUIRE(scene.get_pixel(0, 3, 3) == white);
}
