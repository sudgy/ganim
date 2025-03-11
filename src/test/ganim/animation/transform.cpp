#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/transform.hpp"
#include "ganim/object/shape.hpp"
#include "ganim/object/polygon_shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("texture_transform basics", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    // {-3, -3} and {3, 3} are there to make their bounding boxes the same so
    // that no funky scaling happens
    auto shape1 = make_shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  3}, {-1, -3}, { 1,  3}, { 1, -3}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2);
    shape1->set_visible(true);
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
    auto shape1 = make_shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-3,  9}, {-3, -9}, { 3,  9}, { 3, -9}, {-9, -9}, {9, 9}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2);
    shape1->set_visible(true);
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

TEST_CASE("texture_transform with a static color", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    auto shape1 = make_shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  3}, {-1, -3}, { 1,  3}, { 1, -3}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    const auto black = Color("000000");
    const auto orange = Color("FF8000");
    shape1->set_color(orange);
    shape2->set_color(orange);
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 0 and x < 7 and y > 2 and y < 5) color = orange;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 1 and x < 6 and y > 2 and y < 5) color = orange;
            if (x > 2 and x < 5 and y > 1 and y < 6) color = orange;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 2 and x < 5 and y > 0 and y < 7) color = orange;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform with a changing color", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    auto shape1 = make_shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  3}, {-1, -3}, { 1,  3}, { 1, -3}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    const auto white = Color("FFFFFF");
    const auto black = Color("000000");
    const auto dark_red = Color("800000");
    const auto redish = Color("C08080");
    shape1->set_color(white);
    shape2->set_color(dark_red);
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 0 and x < 7 and y > 2 and y < 5) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            // I'm not sure if this is the final algorithm I want for colors in
            // texture_transform, so feel free to change this test later if you
            // change it
            color = black;
            if (x > 1 and x < 6 and y > 2 and y < 5) color = white;
            if (x > 2 and x < 5 and y > 1 and y < 6) color = dark_red;
            if (x > 2 and x < 5 and y > 2 and y < 5) color = redish;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 2 and x < 5 and y > 0 and y < 7) color = dark_red;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform moving", "[animation]") {
    auto scene = TestScene(4, 4, 4, 4, 2);
    auto shape1 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    shape1->shift(-vga2::e1);
    shape2->shift(vga2::e1);
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x < 2 and y > 0 and y < 3) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 0 and x < 3 and y > 0 and y < 3) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 1 and x < 4 and y > 0 and y < 3) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform moving with direction", "[animation]") {
    auto scene = TestScene(4, 4, 4, 4, 2);
    auto shape1 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    shape1->shift(-vga2::e1);
    shape2->shift(vga2::e1);
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2, {.direction = vga3::e2});
    scene.wait(1);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x < 2 and y > 0 and y < 3) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 0 and x < 3 and y < 2) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 1 and x < 4 and y > 0 and y < 3) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform multiple times on the same object", "[animation]")
{
    auto scene = TestScene(4, 6, 4, 6, 2);
    auto shape1 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape3 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    shape1->shift(-vga2::e1);
    shape2->shift(vga2::e1 + 2*vga2::e2);
    shape3->shift(vga2::e1 - 2*vga2::e2);
    scene.add(shape1, shape2, shape3);
    shape1->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    texture_transform(scene, shape1, shape3);
    scene.wait(1);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 6; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x < 2 and y > 1 and y < 4) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 0 and x < 3 and y > 0 and y < 5) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 1 and y < 2) color = white;
            if (x > 1 and y > 3) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform multiple times to the same object", "[animation]")
{
    auto scene = TestScene(4, 6, 4, 6, 2);
    auto shape1 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape3 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    shape1->shift(-vga2::e1);
    shape2->shift(vga2::e1 + 2*vga2::e2);
    shape3->shift(vga2::e1 - 2*vga2::e2);
    scene.add(shape1, shape2, shape3);
    shape2->set_visible(true);
    shape3->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, shape2, shape1);
    texture_transform(scene, shape3, shape1);
    scene.wait(1);
    scene.frame_advance();
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 6; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x > 1 and y < 2) color = white;
            if (x > 1 and y > 3) color = white;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x > 0 and x < 3 and y > 0 and y < 5) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x < 2 and y > 1 and y < 4) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE(
    "texture_transform objects before they finished transforming/animating",
    "[animation]"
)
{
    auto scene = TestScene(4, 4, 4, 4, 2);
    auto shape1 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape3 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape4 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape5 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2, shape3, shape4, shape5);
    texture_transform(scene, shape1, shape2);
    REQUIRE_THROWS(texture_transform(scene, shape2, shape3));
    REQUIRE_THROWS(animate(scene, shape2));
    animate(scene, shape4);
    REQUIRE_THROWS(texture_transform(scene, shape4, shape5));
}

TEST_CASE("texture_transform multiple times in a row", "[animation]") {
    auto scene = TestScene(4, 4, 4, 4, 2);
    auto shape1 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape3 = make_shape(
        {{-1,  1}, {-1, -1}, { 1,  1}, { 1, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2, shape3);
    texture_transform(scene, shape1, shape2);
    scene.wait();
    texture_transform(scene, shape2, shape3);
    scene.wait();
}

TEST_CASE("texture_transform with groups", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    auto shape1 = make_shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  3}, {-1, -3}, { 1,  3}, { 1, -3}, {-3, -3}, {3, 3}},
        {0, 1, 2, 2, 1, 3}
    );
    auto group1 = make_group(shape1);
    auto group2 = make_group(shape2);
    group1->draw_together();
    group2->draw_together();
    scene.add(group1, group2);
    group1->set_visible(true);
    scene.frame_advance();
    texture_transform(scene, group1, group2);
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

// This was a bug where texture_transform was sometimes only drawing the outline
TEST_CASE("texture_transform with outline", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    auto shape1 = make_shape(
        {{-2,  2}, {-2, -2}, { 2,  2}, { 2, -2}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-2,  2}, {-2, -2}, { 2,  2}, { 2, -2}},
        {0, 1, 2, 2, 1, 3}
    );
    const auto red = Color("FF0000");
    const auto white = Color("FFFFFF");
    shape1->set_outline(red, 2);
    shape2->set_outline(red, 2);
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    REQUIRE(scene.get_pixel(0, 4, 4) == white);
}

TEST_CASE("texture_transform with moving outline", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 3);
    auto shape1 = make_shape(
        {{-9,  1}, {-9, -1}, {-7,  1}, {-7, -1}, {5, 0}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-9,  1}, {-9, -1}, { 5,  1}, { 5, -1}},
        {0, 1, 2, 2, 1, 3}
    );
    const auto red = Color("FF0000");
    const auto black = Color("000000");
    shape1->set_outline(red, 2);
    shape2->set_outline(red, 2);
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    texture_transform(scene, shape1, shape2,
            {.rate_function = [](double x) {return x;}});
    scene.wait(1);
    REQUIRE(scene.get_pixel(0, 0, 2) == red);
    REQUIRE(scene.get_pixel(0, 3, 2) == black);
    REQUIRE(scene.get_pixel(0, 7, 2) == black);
    REQUIRE(scene.get_pixel(1, 0, 2) == red);
    REQUIRE(scene.get_pixel(1, 3, 2) == red);
    REQUIRE(scene.get_pixel(1, 7, 2) == black);
    REQUIRE(scene.get_pixel(2, 0, 2) == red);
    REQUIRE(scene.get_pixel(2, 3, 2) == red);
    REQUIRE(scene.get_pixel(2, 7, 2) == red);
}

TEST_CASE("group_transform", "[animation]") {
    auto scene = TestScene(4, 8, 4, 8, 2);
    auto shape1 = make_shape(
        {{-0.1, -0.1}, {-0.1, 1.1}, {1.1, 1.1}, {1.1, -0.1}},
        {0, 1, 2, 2, 0, 3}
    );
    auto shape2 = make_shape(
        {{-0.1, -0.1}, {-0.1, 1.1}, {1.1, 1.1}, {1.1, -0.1}},
        {0, 1, 2, 2, 0, 3}
    );
    auto shape3 = make_shape(
        {{-0.1, -0.1}, {-0.1, 1.1}, {1.1, 1.1}, {1.1, -0.1}},
        {0, 1, 2, 2, 0, 3}
    );
    auto shape4 = make_shape(
        {{-0.1, -0.1}, {-0.1, 1.1}, {1.1, 1.1}, {1.1, -0.1}},
        {0, 1, 2, 2, 0, 3}
    );
    auto shape5 = make_shape(
        {{-0.1, -0.1}, {-0.1, 1.1}, {1.1, 1.1}, {1.1, -0.1}},
        {0, 1, 2, 2, 0, 3}
    );
    auto shape6 = make_shape(
        {{-0.1, -0.1}, {-0.1, 1.1}, {1.1, 1.1}, {1.1, -0.1}},
        {0, 1, 2, 2, 0, 3}
    );
    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    const auto red = Color("FF0000");
    const auto green = Color("00FF00");
    const auto blue = Color("0000FF");
    const auto yellow = Color("FFFF00");
    const auto cyan = Color("00FFFF");
    using namespace vga2;
    shape1->set_color(white).shift(-1*e1 + 1*e2);
    shape2->set_color(red).shift(-1*e1 - 2*e2);
    shape3->set_color(green).shift(1*e1 + 3*e2);
    shape4->set_color(blue).shift(1*e1 - 1*e2);
    shape5->set_color(yellow).shift(1*e1);
    shape6->set_color(cyan).shift(1*e1 - 4*e2);
    auto group1 = make_group(shape1, shape2);
    auto group2 = make_group(shape3, shape4, shape5, shape6);
    scene.add(group1, group2);
    group1->set_visible(true);
    scene.frame_advance();
    group_transform(scene, group1, group2);
    scene.wait();
    scene.frame_advance();

    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);

            auto color = black;
            if (x == 1 and y == 2) color = white;
            if (x == 1 and y == 5) color = red;
            REQUIRE(scene.get_pixel(0, x, y) == color);

            color = black;
            if (x == 2 and y == 1) color = Color("80FF80");
            if (x == 2 and y == 3) color = Color("8080FF");
            if (x == 2 and y == 4) color = Color("FF8000");
            if (x == 2 and y == 6) color = Color("808080");
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x == 3 and y == 0) color = green;
            if (x == 3 and y == 4) color = blue;
            if (x == 3 and y == 3) color = yellow;
            if (x == 3 and y == 7) color = cyan;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("Global transform basic", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    auto shape1 = make_shape(
        {{-3,  1}, {-3, -1}, { 3,  1}, { 3, -1}},
        {0, 1, 2, 2, 1, 3}
    );
    auto shape2 = make_shape(
        {{-1,  3}, {-1, -3}, { 1,  3}, { 1, -3}},
        {0, 1, 2, 2, 1, 3}
    );
    scene.add(shape1, shape2);
    shape1->set_visible(true);
    scene.frame_advance();
    transform(scene, shape1, shape2);
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
            if (x > 1 and x < 6 and y > 1 and y < 6) color = white;
            REQUIRE(scene.get_pixel(1, x, y) == color);

            color = black;
            if (x > 2 and x < 5 and y > 0 and y < 7) color = white;
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("texture_transform during depth peeling", "[animation]") {
    auto scene = TestScene(8, 8, 8, 8, 2);
    scene.set_transparency_layers(1);
    using namespace vga2;
    auto shape1 = make_polygon_shape({
        -10*e1 - 10*e2,
         10*e1 - 10*e2,
         10*e1 + 10*e2,
        -10*e1 + 10*e2
    });
    shape1->set_opacity(0.5);
    auto shape2 = shape1.copy_object();
    shape1->set_visible(true);
    scene.add(shape1);
    scene.frame_advance();
    texture_transform(scene, shape1, shape2);
    scene.wait(1);
    const auto color = Color("7F7F7F");
    REQUIRE(scene.get_pixel(0, 3, 3) == color);
    REQUIRE(scene.get_pixel(1, 3, 3) == color);
    REQUIRE(scene.get_pixel(2, 3, 3) == color);
}
