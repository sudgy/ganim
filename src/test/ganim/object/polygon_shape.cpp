#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/polygon_shape.hpp"

#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("PolygonShape", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 1);
    using namespace vga2;
    auto shape = make_polygon_shape(std::vector{
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
    shape->set_visible(true);
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

TEST_CASE("depth_z", "[object]") {
    auto scene = TestScene(8, 8, 8, 8, 1);
    using namespace vga2;
    auto shape1 = make_polygon_shape({
        -10*e1 - 10*e2,
         10*e1 - 10*e2,
         10*e1 + 10*e2,
        -10*e1 + 10*e2
    });
    auto shape2 = shape1.copy_object();
    shape2->set_color("FF0000");
    shape2->shift(-0.5*vga3::e3);
    shape2->set_depth_z(1);
    shape1->set_visible(true);
    shape2->set_visible(true);
    scene.add(shape1, shape2);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 3, 3) == "FF0000");
}

TEST_CASE("PolygonShape 3D", "[object]") {
    auto scene = TestScene(8, 8, 4, 4, 1);
    using namespace vga3;
    auto shape1 = make_polygon_shape({
        -3*e1 - 3*e2 - 2*e3,
         3*e1 - 3*e2 - 2*e3,
         3*e1 + 3*e2 + 2*e3,
        -3*e1 + 3*e2 + 2*e3,
    });
    auto shape2 = make_shape(
        {
            {-3, -3, -2},
            { 3, -3, -2},
            { 3,  3,  2},
            {-3,  3,  2}
        },
        {0, 1, 2, 0, 2, 3}
    );
    scene.check_draw_equivalent(shape1, shape2);
}

TEST_CASE("fixed_orientation", "[object]") {
    auto scene = TestScene(8, 8, 4, 4, 1);
    using namespace vga3;
    auto shape1 = make_polygon_shape({
        -e1 - e2,
         e1 - e2,
         e1 + e2,
        -e1 + e2
    });
    shape1->set_fixed_orientation(true);
    auto shape2 = make_polygon_shape({
        -e1 - e2,
         e1 - e2,
         e1 + e2,
        -e1 + e2
    });
    scene.check_draw_equivalent(shape1, shape2, "1");
    scene.get_camera()->rotate(τ/4, e23);
    shape2->rotate(τ/4, e23);
    scene.check_draw_equivalent(shape1, shape2, "2");
    scene.get_camera()->rotate(τ/8, e13);
    shape2->rotate(τ/8, e13);
    scene.check_draw_equivalent(shape1, shape2, "3");
    shape1->shift(e1);
    shape2->shift(e1);
    scene.check_draw_equivalent(shape1, shape2, "4");
    scene.get_camera()->shift(e1);
    scene.check_draw_equivalent(shape1, shape2, "5");
}
