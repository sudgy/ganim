#include <catch2/catch_test_macros.hpp>

#include "test_scene.hpp"

TEST_CASE("Scene basics", "[scene]") {
    auto scene = TestScene(10, 10);
    scene.frame_advance();
    scene.frame_advance();
    scene.frame_advance();
    REQUIRE(scene.time_size() == 3);
    auto p = scene.get_pixel(0, 0, 0);
    REQUIRE(p.r == 0);
    REQUIRE(p.g == 0);
    REQUIRE(p.b == 0);
    REQUIRE(p.a == 255);
    p = scene.get_pixel(0, 8, 8);
    REQUIRE(p.r == 0);
    REQUIRE(p.g == 0);
    REQUIRE(p.b == 0);
    REQUIRE(p.a == 255);
    p = scene.get_pixel(2, 5, 5);
    REQUIRE(p.r == 0);
    REQUIRE(p.g == 0);
    REQUIRE(p.b == 0);
    REQUIRE(p.a == 255);
}
