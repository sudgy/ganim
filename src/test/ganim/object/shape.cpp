#include <catch2/catch_test_macros.hpp>

#include "ganim/object/shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("Shape drawing", "[scene]") {
    auto scene = TestScene(10, 10, 10, 10, 1);
    auto shape = Shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 3, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 6, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 6, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 2, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 3, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 3, 7) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 7, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 6, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 7, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 6, 7) == Color("000000"));
}

TEST_CASE("Shape perspective", "[scene]") {
    auto scene = TestScene(20, 20, 5, 5, 1);
    auto shape = Shape(
        {{ 2,  2,  2},
         { 2, -2, -2},
         {-2, -2, -2},
         {-2,  2,  2}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 1, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 1, 9) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 1, 15) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 2, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 2, 15) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 3, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 15) == Color("FFFFFF"));

    REQUIRE(scene.get_pixel(0, 18, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 18, 9) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 18, 15) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 17, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 17, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 17, 15) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 16, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 16, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 16, 15) == Color("FFFFFF"));
}

TEST_CASE("Shape behind camera", "[scene]") {
    auto scene = TestScene(5, 5, 1, 1, 1);
    auto shape = Shape(
        {{ 2,  2, 30},
         { 2, -2, 30},
         {-2, -2, 30},
         {-2,  2, 30}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 4, 4) == Color("000000"));
}
