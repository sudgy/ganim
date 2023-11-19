#include <catch2/catch_test_macros.hpp>

#include "ganim/object/shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("Shape drawing", "[object]") {
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

TEST_CASE("Shape perspective", "[object]") {
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

TEST_CASE("Shape behind camera", "[object]") {
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

TEST_CASE("Shape moving", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto shape = Shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    shape.shift(e1);
    scene.frame_advance();
    scene.get_camera().shift(e2);
    scene.frame_advance();
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x >= 2 and (y == 1 or y == 2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(0, x, y) == color);
            color = Color("000000");
            if (x >= 2 and y >= 2) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(1, x, y) == color);
        }
    }
}

TEST_CASE("Shape color", "[object]") {
    auto scene = TestScene(3, 3, 2, 2, 1);
    auto shape = Shape(
        {{ 1,  1, 0, 1, 0, 0, 1},
         { 1, -1, 0, 1, 0, 0, 1},
         {-1, -1, 0, 0, 1, 0, 1},
         {-1,  1, 0, 0, 1, 0, 1}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    scene.frame_advance();
    // I guess OpenGL starts interpolating on the edges instantly?  I hope this
    // isn't system-dependent
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("2AD400"));
    REQUIRE(scene.get_pixel(0, 1, 0) == Color("807F00"));
    REQUIRE(scene.get_pixel(0, 2, 0) == Color("D42A00"));
}

TEST_CASE("Shape depth test", "[object]") {
    using namespace vga3;
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto red = Shape(
        {{ 1,  1, 0, 1, 0, 0, 1},
         { 1, -1, 0, 1, 0, 0, 1},
         {-1, -1, 0, 1, 0, 0, 1},
         {-1,  1, 0, 1, 0, 0, 1}},
        {0, 1, 2, 0, 2, 3}
    );
    auto green = Shape(
        {{ 1,  1, 0, 0, 1, 0, 1},
         { 1, -1, 0, 0, 1, 0, 1},
         {-1, -1, 0, 0, 1, 0, 1},
         {-1,  1, 0, 0, 1, 0, 1}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(red, green);
    scene.frame_advance();
    green.shift(e3);
    scene.frame_advance();
    green.shift(-2*e3);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("00FF00"));
    REQUIRE(scene.get_pixel(1, 0, 0) == Color("00FF00"));
    REQUIRE(scene.get_pixel(2, 0, 0) == Color("FF0000"));
}

TEST_CASE("Shape animation", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto shape = Shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    shape.shift(-e1);
    scene.frame_advance();
    shape.animate(2, [](double x){return x;});
    shape.shift(2*e1);
    scene.frame_advance(3);
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x < 2 and (y == 1 or y == 2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(0, x, y) == color);
            color = Color("000000");
            if ((x==1 or x==2) and (y==1 or y==2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(1, x, y) == color);
            color = Color("000000");
            if (x >= 2 and (y == 1 or y == 2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("Shape camera animation", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto shape = Shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    shape.shift(-e1);
    scene.frame_advance();
    scene.get_camera().animate(2, [](double x){return x;});
    scene.get_camera().shift(-2*e1);
    scene.frame_advance(3);
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x < 2 and (y == 1 or y == 2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(0, x, y) == color);
            color = Color("000000");
            if ((x==1 or x==2) and (y==1 or y==2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(1, x, y) == color);
            color = Color("000000");
            if (x >= 2 and (y == 1 or y == 2)) color = Color("FFFFFF");
            REQUIRE(scene.get_pixel(2, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
}

TEST_CASE("Shape opacity", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto solid = Shape(
        {{ 1,  1, 0, 1, 1, 1, 1},
         { 1, -1, 0, 1, 1, 1, 1},
         {-1, -1, 0, 1, 1, 1, 1},
         {-1,  1, 0, 1, 1, 1, 1}},
        {0, 1, 2, 0, 2, 3}
    );
    solid.shift(e1);
    auto red_trans = Shape(
        {{ 1,  1, 0, 1, 0, 0, 0.5},
         { 1, -1, 0, 1, 0, 0, 0.5},
         {-1, -1, 0, 1, 0, 0, 0.5},
         {-1,  1, 0, 1, 0, 0, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto invisible = Shape(
        {{ 1,  1, 0, 1, 1, 1, 0},
         { 1, -1, 0, 1, 1, 1, 0},
         {-1, -1, 0, 1, 1, 1, 0},
         {-1,  1, 0, 1, 1, 1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(solid, red_trans, invisible);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 1, 1) == Color("800000"));
    REQUIRE(scene.get_pixel(0, 2, 1) == Color("FF8080"));
    REQUIRE(scene.get_pixel(0, 3, 1) == Color("FFFFFF"));
}
