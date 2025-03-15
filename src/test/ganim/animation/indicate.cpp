#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/indicate.hpp"

#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/ga_equals.hpp"
#include "ganim/object/shape.hpp"

using namespace ganim;

TEST_CASE("Indicate", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 4);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    indicate(scene, shape, {1, rf::linear, 2});
    scene.wait(1);
    auto black = Color("000000");
    auto white = Color("FFFFFF");
    auto yellow = Color("FFFF00");
    auto yellowish = Color("FFFF7F");
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 1 and x < 6 and y > 1 and y < 6) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);
            REQUIRE(scene.get_pixel(4, x, y) == color);

            color = black;
            if (x > 0 and x < 7 and y > 0 and y < 7) color = yellowish;
            REQUIRE(scene.get_pixel(1, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);

            REQUIRE(scene.get_pixel(2, x, y) == yellow);
        }
    }
}

TEST_CASE("Indicate custom color", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 4);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    auto red = Color("FF0000");
    indicate(scene, shape, {1, rf::linear, 2, red});
    scene.wait(1);
    auto black = Color("000000");
    auto white = Color("FFFFFF");
    auto reddish = Color("FF7F7F");
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 1 and x < 6 and y > 1 and y < 6) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);
            REQUIRE(scene.get_pixel(4, x, y) == color);

            color = black;
            if (x > 0 and x < 7 and y > 0 and y < 7) color = reddish;
            REQUIRE(scene.get_pixel(1, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);

            REQUIRE(scene.get_pixel(2, x, y) == red);
        }
    }
}

TEST_CASE("Indicate 3D", "[animation]") {
    auto scene = TestScene(1, 1, 1, 1, 2);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    shape->shift(vga3::e3);
    auto r = shape->get_rotor();
    indicate(scene, shape, {.scale_factor = 2});
    scene.frame_advance();
    REQUIRE_THAT(r, GAEquals(shape->get_rotor()));
    REQUIRE(shape->get_scale() == 2);
}
