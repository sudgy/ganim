#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/creation.hpp"
#include "ganim/object/shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("Shape creating", "[animation]") {
    auto scene = TestScene(16, 1, 4, 2, 4);
    auto shape = make_shape(
        {{-1,  1, 0, 0},
         {-1, -1, 0, 0},
         { 0,  1, 0, 1},
         { 0, -1, 0, 1},
         { 2,  1, 0, 2},
         { 2, -1, 0, 2}},
        {0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 5}
    );
    scene.add(shape);
    create(scene, shape, {.rate_function = [](double t) {return t;}});
    scene.wait();
    REQUIRE(scene.get_pixel(0, 4, 0) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 6, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 6, 0) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(1, 8, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 8, 0) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(2, 12, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 12, 0) == Color("FFFFFF"));
}
