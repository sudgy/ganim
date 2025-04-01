#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/scaling.hpp"
#include "ganim/object/shape.hpp"
#include "ganim/object/polygon_shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("grow_from_point and shrink_to_point", "[animation]") {
    auto scene = TestScene(16, 8, 16, 8, 4);
    auto shape = make_shape(
        {{-4,  4},
         {-4, -4},
         { 4,  4},
         { 4, -4}},
         {0, 1, 2, 1, 2, 3}
    );
    scene.add(shape);
    scene.frame_advance();
    grow_from_point(
        scene,
        shape,
        {.rate_function = rf::linear, .about_point = -8*vga3::e1}
    );
    scene.wait();
    shrink_to_point(
        scene,
        shape,
        {.rate_function = rf::linear, .about_point = -8*vga3::e1}
    );
    scene.wait();
    scene.frame_advance();
    shape->set_visible(true);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            REQUIRE(scene.get_pixel(0, x, y) == color);
            REQUIRE(scene.get_pixel(8, x, y) == color);
            REQUIRE(scene.get_pixel(9, x, y) == color);

            if (x > 0 and x < 3 and y > 2 and y < 5) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);
            REQUIRE(scene.get_pixel(7, x, y) == color);

            color = black;
            if (x > 1 and x < 6 and y > 1 and y < 6) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(6, x, y) == color);

            color = black;
            if (x > 2 and x < 9 and y > 0 and y < 7) color = white;
            REQUIRE(scene.get_pixel(3, x, y) == color);
            REQUIRE(scene.get_pixel(5, x, y) == color);

            color = black;
            if (x > 3 and x < 12) color = white;
            REQUIRE(scene.get_pixel(4, x, y) == color);
            REQUIRE(scene.get_pixel(10, x, y) == color);
        }
    }
}

TEST_CASE("shrink_in", "[animation]") {
    using namespace vga2;
    auto scene = TestScene(16, 16, 16, 16, 4);
    auto shape = make_polygon_shape({
        -4*e1 - 4*e2, 4*e1 - 4*e2, 4*e1 + 4*e2, -4*e1 + 4*e2
    });
    shrink_in(scene, shape, {.rate_function = rf::linear, .scale_factor = 2});
    auto part1 = make_polygon_shape({
        -7*e1 - 7*e2, 7*e1 - 7*e2, 7*e1 + 7*e2, -7*e1 + 7*e2
    });
    part1->set_color("0F0F0F");
    auto part2 = make_polygon_shape({
        -6*e1 - 6*e2, 6*e1 - 6*e2, 6*e1 + 6*e2, -6*e1 + 6*e2
    });
    part2->set_color("3F3F3F");
    auto part3 = make_polygon_shape({
        -5*e1 - 5*e2, 5*e1 - 5*e2, 5*e1 + 5*e2, -5*e1 + 5*e2
    });
    part3->set_color("8F8F8F");
    auto part4 = make_polygon_shape({
        -4*e1 - 4*e2, 4*e1 - 4*e2, 4*e1 + 4*e2, -4*e1 + 4*e2
    });
    scene.wait(1);

    shape.reset();
    scene.add(part1);
    part1->set_visible(true);
    scene.frame_advance();

    part1.reset();
    scene.add(part2);
    part2->set_visible(true);
    scene.frame_advance();

    part2.reset();
    scene.add(part3);
    part3->set_visible(true);
    scene.frame_advance();

    part3.reset();
    scene.add(part4);
    part4->set_visible(true);
    scene.frame_advance();

    for (int i = 0; i < 4; ++i) {
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                INFO("Frame " << i + 1);
                INFO("x: " << x << " y: " << y);
                REQUIRE(scene.get_pixel(i, x, y) == scene.get_pixel(i+4, x, y));
            }
        }
    }
}
