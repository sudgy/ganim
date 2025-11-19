#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ganim/object/video.hpp"
#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"

using namespace ganim;
using namespace Catch::Matchers;

TEST_CASE("Video drawing", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 4);
    auto test = make_video("test_files/test_video.mp4");
    test->scale(120.0/32.0);
    REQUIRE_THAT(test->get_width(), WithinRel(2, 1e-5));
    test->set_visible(true);
    scene.add(test);
    scene.frame_advance(7);
    auto black = Color("000000");
    for (int i = 0; i <= 6; ++i) {
        INFO("i = " << i);
        auto color1 = Color("FF0000");
        auto color2 = Color("00FF00");
        auto color3 = Color("0000FF");
        auto color4 = Color("FFFFFF");
        if (i >= 2 and i < 4) {
            color1 = Color("FFFF00");
            color2 = Color("00FFFF");
            color3 = Color("FF00FF");
            color4 = Color("000000");
        }
        else if (i == 4) {
            color1 = Color("800000");
            color2 = Color("008000");
            color3 = Color("000080");
            color4 = Color("808080");
        }
        else if (i == 5) {
            color1 = Color("808000");
            color2 = Color("008080");
            color3 = Color("800080");
            color4 = Color("000000");
        }
        else if (i == 6) {
            color1 = color2 = color3 = color4 = black;
        }
        REQUIRE(scene.get_pixel(i, 0, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 2, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 3, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 1) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 1) == ApproxColor(color1));
        REQUIRE(scene.get_pixel(i, 2, 1) == ApproxColor(color2));
        REQUIRE(scene.get_pixel(i, 3, 1) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 2) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 2) == ApproxColor(color3));
        REQUIRE(scene.get_pixel(i, 2, 2) == ApproxColor(color4));
        REQUIRE(scene.get_pixel(i, 3, 2) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 2, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 3, 3) == ApproxColor(black));
    }
}

TEST_CASE("Video different frame rate", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 2);
    auto test = make_video("test_files/test_video.mp4");
    test->scale(120.0/32.0);
    test->set_visible(true);
    scene.add(test);
    scene.frame_advance(4);
    auto black = Color("000000");
    for (int i = 0; i <= 3; ++i) {
        if (i == 2) continue;
        INFO("i = " << i);
        auto color1 = Color("FF0000");
        auto color2 = Color("00FF00");
        auto color3 = Color("0000FF");
        auto color4 = Color("FFFFFF");
        if (i == 1) {
            color1 = Color("FFFF00");
            color2 = Color("00FFFF");
            color3 = Color("FF00FF");
            color4 = Color("000000");
        }
        else if (i == 3) {
            color1 = color2 = color3 = color4 = black;
        }
        REQUIRE(scene.get_pixel(i, 0, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 2, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 3, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 1) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 1) == ApproxColor(color1));
        REQUIRE(scene.get_pixel(i, 2, 1) == ApproxColor(color2));
        REQUIRE(scene.get_pixel(i, 3, 1) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 2) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 2) == ApproxColor(color3));
        REQUIRE(scene.get_pixel(i, 2, 2) == ApproxColor(color4));
        REQUIRE(scene.get_pixel(i, 3, 2) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 2, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 3, 3) == ApproxColor(black));
    }
}

TEST_CASE("Video speed up", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 4);
    auto test = make_video("test_files/test_video.mp4");
    test->set_speed(2.0);
    test->scale(120.0/32.0);
    test->set_visible(true);
    scene.add(test);
    scene.frame_advance(4);
    auto black = Color("000000");
    for (int i = 0; i <= 3; ++i) {
        if (i == 2) continue;
        INFO("i = " << i);
        auto color1 = Color("FF0000");
        auto color2 = Color("00FF00");
        auto color3 = Color("0000FF");
        auto color4 = Color("FFFFFF");
        if (i == 1) {
            color1 = Color("FFFF00");
            color2 = Color("00FFFF");
            color3 = Color("FF00FF");
            color4 = Color("000000");
        }
        else if (i == 3) {
            color1 = color2 = color3 = color4 = black;
        }
        REQUIRE(scene.get_pixel(i, 0, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 2, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 3, 0) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 1) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 1) == ApproxColor(color1));
        REQUIRE(scene.get_pixel(i, 2, 1) == ApproxColor(color2));
        REQUIRE(scene.get_pixel(i, 3, 1) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 2) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 2) == ApproxColor(color3));
        REQUIRE(scene.get_pixel(i, 2, 2) == ApproxColor(color4));
        REQUIRE(scene.get_pixel(i, 3, 2) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 0, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 1, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 2, 3) == ApproxColor(black));
        REQUIRE(scene.get_pixel(i, 3, 3) == ApproxColor(black));
    }
}
