#include <catch2/catch_test_macros.hpp>

#include "test_scene.hpp"

using namespace ganim;

TEST_CASE("Scene basics", "[scene]") {
    auto scene = TestScene(10, 10, 1);
    scene.frame_advance();
    scene.frame_advance();
    scene.frame_advance();
    REQUIRE(scene.time_size() == 3);
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 8, 8) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 5, 5) == Color("000000"));
}

TEST_CASE("Scene background color", "[scene]") {
    auto scene = TestScene(1, 1, 1);
    scene.set_background_color({255, 0, 0, 255});
    scene.frame_advance();
    scene.set_background_color({0, 255, 0, 255});
    scene.frame_advance();
    scene.set_background_color({0, 0, 127, 0});
    scene.frame_advance();
    REQUIRE(scene.time_size() == 3);
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("FF0000"));
    REQUIRE(scene.get_pixel(1, 0, 0) == Color("00FF00"));
    REQUIRE(scene.get_pixel(2, 0, 0) == Color("00007F"));
}

TEST_CASE("Scene waiting", "[scene]") {
    auto scene = TestScene(1, 1, 4);
    scene.frame_advance(3);
    REQUIRE(scene.time_size() == 3);
    scene.wait(0.5);
    REQUIRE(scene.time_size() == 5);
    scene.wait(0.7);
    REQUIRE(scene.time_size() == 8);
    scene.wait(0.8);
    REQUIRE(scene.time_size() == 11);
    REQUIRE_THROWS(scene.frame_advance(-1));
    REQUIRE_THROWS(scene.wait(-0.1));
}