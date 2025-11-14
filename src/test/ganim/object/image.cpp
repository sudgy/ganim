#include <catch2/catch_test_macros.hpp>

#include "ganim/object/image.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("Image drawing", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto test = make_image("test_files/image.png");
    test->scale(120.0);
    test->set_visible(true);
    scene.add(test);
    scene.frame_advance();
    auto black = Color("000000");
    REQUIRE(scene.get_pixel(0, 0, 0) == black);
    REQUIRE(scene.get_pixel(0, 1, 0) == black);
    REQUIRE(scene.get_pixel(0, 2, 0) == black);
    REQUIRE(scene.get_pixel(0, 3, 0) == black);
    REQUIRE(scene.get_pixel(0, 0, 1) == black);
    REQUIRE(scene.get_pixel(0, 1, 1) == Color("00FF00"));
    REQUIRE(scene.get_pixel(0, 2, 1) == Color("FF0000"));
    REQUIRE(scene.get_pixel(0, 3, 1) == black);
    REQUIRE(scene.get_pixel(0, 0, 2) == black);
    REQUIRE(scene.get_pixel(0, 1, 2) == Color("0000FF"));
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 2) == black);
    REQUIRE(scene.get_pixel(0, 0, 3) == black);
    REQUIRE(scene.get_pixel(0, 1, 3) == black);
    REQUIRE(scene.get_pixel(0, 2, 3) == black);
    REQUIRE(scene.get_pixel(0, 3, 3) == black);
}
