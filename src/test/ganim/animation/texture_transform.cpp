#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/texture_transform.hpp"
#include "ganim/object/shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("texture_transform basics", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    // {-3, -3} and {3, 3} are there to make their bounding boxes the same so
    // that no funky scaling happens
    auto shape1 = Shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = Shape(
        {{-1,  3}, {-1, -3}, { 1,  3}, { 1, -3}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2);
    shape1.set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 0 and x < 7 and y > 2 and y < 5) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 1 and x < 6 and y > 2 and y < 5) color = white;
            if (x > 2 and x < 5 and y > 1 and y < 6) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 2 and x < 5 and y > 0 and y < 7) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform changing bounding box", "[animation]") {
    auto scene = TestScene(24, 24, 24, 24, 2);
    auto shape1 = Shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = Shape(
        {{-3,  9}, {-3, -9}, { 3,  9}, { 3, -9}, {-9, -9}, {9, 9}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2);
    shape1.set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 24; ++x) {
        for (int y = 0; y < 24; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 8 and x < 15 and y > 10 and y < 13) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 7 and x < 16 and y > 9 and y < 14) color = white;
            // I honestly don't know why this is y in (8, 15) rather than (7,
            // 16).  I hope it's a rounding error.
            if (x > 9 and x < 14 and y > 8 and y < 15) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 8 and x < 15 and y > 2 and y < 21) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}
