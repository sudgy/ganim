#include <catch2/catch_test_macros.hpp>

#include "ganim/object/shape.hpp"
#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"

#include "ganim/animation/creation.hpp"

using namespace ganim;

TEST_CASE("Outlines", "[object]") {
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto shape = Shape(
        {{ 0, 0, 0, 0, 1, 1, 1, 1},
         { 1, 0, 0, 0, 1, 1, 1, 1},
         { 1, 1, 0, 0, 1, 1, 1, 1},
         { 0, 1, 0, 0, 1, 1, 1, 1},
         {-1, 0, 0, 0, 1, 1, 1, 0.01},
         { 0, 0, 0, 0, 1, 1, 1, 0.01},
         { 0, 1, 0, 0, 1, 1, 1, 0.01},
         {-1, 1, 0, 0, 1, 1, 1, 0.01},
         // This coordinate is unused when drawing, but it makes the bounding
         // box have an even height, making this test easier to design
         {-1, 2}},
        {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7}
    );
    shape.set_outline(Color("#FF0000"), 3);
    shape.set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    const auto red = std::array<unsigned char, 8*8>{
        0,   0,   0,  86, 128,  86,   0,   0,
        0,   0, 171, 255, 255, 255, 171,   0,
        0,  86, 255, 255, 255, 255, 255,  86,
        0, 128, 255, 255, 255, 255, 255, 128,
        0,  86, 255, 255, 255, 255, 255,  86,
        0,   0, 171, 255, 255, 255, 171,   0,
        0,   0,   0,  86, 128,  86,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0
    };
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            const auto color = scene.get_pixel(0, x, y);
            REQUIRE(int(color.r) == int(red[x + y*8]));
            if (y == 3) {
                if (x == 3) {
                    REQUIRE(int(color.g) == 3);
                    REQUIRE(int(color.b) == 3);
                    continue;
                }
                else if (x == 4) {
                    REQUIRE(int(color.g) == 255);
                    REQUIRE(int(color.b) == 255);
                    continue;
                }
            }
            REQUIRE(int(color.g) == 0);
            REQUIRE(int(color.b) == 0);
        }
    }
}

TEST_CASE("Outline creating", "[object]") {
    auto scene = TestScene(16, 16, 4, 4, 4);
    auto shape = Shape(
        {{-1,  1, 0, 0},
         {-1, -1, 0, 0},
         { 0,  1, 0, 1},
         { 0, -1, 0, 1},
         { 2,  1, 0, 2},
         { 2, -1, 0, 2}},
        {0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 5}
    );
    scene.add(shape);
    shape.set_outline(Color("#FF0000"), 0.375);
    create(shape, {.rate_function = [](double t) {return t;}});
    scene.wait();
    REQUIRE(scene.get_pixel(0, 4, 3) == Color("FF0000"));
    REQUIRE(scene.get_pixel(0, 7, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 6, 3) == Color("FF0000"));
    REQUIRE(scene.get_pixel(1, 9, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 8, 3) == Color("FF0000"));
    REQUIRE(scene.get_pixel(2, 13, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 11, 3) == Color("FF0000"));
}
