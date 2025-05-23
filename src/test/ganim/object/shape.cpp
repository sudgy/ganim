#include <catch2/catch_test_macros.hpp>

#include "ganim/object/shape.hpp"
#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"
#include "test/ganim/ga_equals.hpp"

#include "ganim/animation/animation.hpp"
#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("Shape drawing", "[object]") {
    auto scene = TestScene(10, 10, 10, 10, 1);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
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
    auto shape = make_shape(
        {{ 2,  2,  2},
         { 2, -2, -2},
         {-2, -2, -2},
         {-2,  2,  2}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->do_shading(false);
    shape->set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 1, 2) != Color("000000"));
    REQUIRE(scene.get_pixel(0, 1, 9) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 1, 15) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 2, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 2, 15) != Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 15) == Color("FFFFFF"));

    REQUIRE(scene.get_pixel(0, 18, 2) != Color("000000"));
    REQUIRE(scene.get_pixel(0, 18, 9) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 18, 15) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 17, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 17, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 17, 15) != Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 16, 2) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 16, 9) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 16, 15) == Color("FFFFFF"));
}

TEST_CASE("Shape behind camera", "[object]") {
    auto scene = TestScene(5, 5, 1, 1, 1);
    auto shape = make_shape(
        {{ 2,  2, 30},
         { 2, -2, 30},
         {-2, -2, 30},
         {-2,  2, 30}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 4, 4) == Color("000000"));
}

TEST_CASE("Shape moving", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto shape = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    shape->shift(e1);
    scene.frame_advance();
    scene.get_camera()->shift(e2);
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
    auto shape = make_shape(
        {{ 1,  1, 0, 0, 1, 0, 0, 1},
         { 1, -1, 0, 0, 1, 0, 0, 1},
         {-1, -1, 0, 0, 0, 1, 0, 1},
         {-1,  1, 0, 0, 0, 1, 0, 1}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    // I guess OpenGL starts interpolating on the edges instantly?  I hope this
    // isn't system-dependent
    REQUIRE(scene.get_pixel(0, 0, 0) == ApproxColor("2AD400"));
    REQUIRE(scene.get_pixel(0, 1, 0) == ApproxColor("807F00"));
    REQUIRE(scene.get_pixel(0, 2, 0) == ApproxColor("D42A00"));
}

TEST_CASE("Shape depth test", "[object]") {
    using namespace vga3;
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto red = make_shape(
        {{ 1,  1, 0, 0, 1, 0, 0, 1},
         { 1, -1, 0, 0, 1, 0, 0, 1},
         {-1, -1, 0, 0, 1, 0, 0, 1},
         {-1,  1, 0, 0, 1, 0, 0, 1}},
         {0, 1, 2, 0, 2, 3}
    );
    auto green = make_shape(
        {{ 1,  1, 0, 0, 0, 1, 0, 1},
         { 1, -1, 0, 0, 0, 1, 0, 1},
         {-1, -1, 0, 0, 0, 1, 0, 1},
         {-1,  1, 0, 0, 0, 1, 0, 1}},
         {0, 1, 2, 0, 2, 3}
    );
    red->set_visible(true);
    green->set_visible(true);
    scene.add(red, green);
    scene.frame_advance();
    green->shift(e3);
    scene.frame_advance();
    green->shift(-2*e3);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("00FF00"));
    REQUIRE(scene.get_pixel(1, 0, 0) == Color("00FF00"));
    REQUIRE(scene.get_pixel(2, 0, 0) == Color("FF0000"));
}

TEST_CASE("Shape animation", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto shape = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    shape->shift(-e1);
    scene.frame_advance();
    animate(scene, shape, {2, [](double x){return x;}}).shift(2*e1);
    static_assert(std::is_same_v<
        decltype(animate(scene, shape).shift(e1)),
        Shape&
    >);
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
    auto shape = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    shape->shift(-e1);
    scene.frame_advance();
    animate(
        scene,
        scene.get_camera(),
        {2, [](double x){return x;}}
    ).shift(-2*e1);
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
    auto solid = make_shape(
        {{ 1,  1, 0, 0, 1, 1, 1, 1},
         { 1, -1, 0, 0, 1, 1, 1, 1},
         {-1, -1, 0, 0, 1, 1, 1, 1},
         {-1,  1, 0, 0, 1, 1, 1, 1}},
         {0, 1, 2, 0, 2, 3}
    );
    solid->shift(e1);
    auto red_trans = make_shape(
        {{ 1,  1, 0, 0, 1, 0, 0, 0.5},
         { 1, -1, 0, 0, 1, 0, 0, 0.5},
         {-1, -1, 0, 0, 1, 0, 0, 0.5},
         {-1,  1, 0, 0, 1, 0, 0, 0.5}},
         {0, 1, 2, 0, 2, 3}
    );
    auto invisible = make_shape(
        {{ 1,  1, 0, 0, 1, 1, 1, 0},
         { 1, -1, 0, 0, 1, 1, 1, 0},
         {-1, -1, 0, 0, 1, 1, 1, 0},
         {-1,  1, 0, 0, 1, 1, 1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    solid->set_visible(true);
    red_trans->set_visible(true);
    invisible->set_visible(true);
    scene.add(solid, red_trans, invisible);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 1, 1) == ApproxColor("800000"));
    REQUIRE(scene.get_pixel(0, 2, 1) == ApproxColor("FF8080"));
    REQUIRE(scene.get_pixel(0, 3, 1) == ApproxColor("FFFFFF"));
}

TEST_CASE("Shape Object color", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto solid = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    solid->shift(e1);
    auto red_trans = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    red_trans->set_color("FF0000");
    red_trans->set_opacity(0.5);
    auto invisible = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    solid->set_visible(true);
    red_trans->set_visible(true);
    invisible->set_visible(true);
    invisible->set_opacity(0);
    scene.add(solid, red_trans, invisible);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 1, 1) == ApproxColor("7F0000"));
    REQUIRE(scene.get_pixel(0, 2, 1) == ApproxColor("FF8080"));
    REQUIRE(scene.get_pixel(0, 3, 1) == ApproxColor("FFFFFF"));
}

TEST_CASE("Shape scaling", "[object]") {
    using namespace vga2;
    auto scene = TestScene(10, 10, 10, 10, 1);
    auto test = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    test->set_visible(true);
    scene.add(test);
    test->shift(e1);
    scene.frame_advance();
    test->scale(2);
    scene.frame_advance();
    test->scale(2, 2*e1);
    scene.frame_advance();
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x >= 5 and x <= 6 and y >= 4 and y <= 5) {
                color = Color("FFFFFF");
            }
            REQUIRE(scene.get_pixel(0, x, y) == color);
            color = Color("000000");
            if (x >= 4 and x <= 7 and y >= 3 and y <= 6) {
                color = Color("FFFFFF");
            }
            REQUIRE(scene.get_pixel(1, x, y) == color);
            color = Color("000000");
            if (x >= 1 and x <= 8 and y >= 1 and y <= 8) {
                color = Color("FFFFFF");
            }
            REQUIRE(scene.get_pixel(2, x, y) == color);
        }
    }
}

// This was a bug I ran into that ended up being about rotor normalization
TEST_CASE("Shape move back and forth with rotation", "[object]") {
    using namespace pga3;
    auto scene = TestScene(4, 4, 4, 4, 30);
    auto shape = make_shape(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    animate(scene, shape).rotate(τ/2, e23);
    scene.wait(1);
    animate(scene, shape).shift(-e1);
    scene.wait(1);
    REQUIRE(scene.get_pixel(59, 1, 1) == Color("FFFFFF"));
}

// This was another bug I ran into that ended up being about rotor normalization
TEST_CASE("Shape rotating too fast?", "[object]") {
    using namespace pga3;
    auto scene = TestScene(4, 4, 4, 4, 60);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    animate(scene, shape).rotate(τ/2, e23).shift(2*e1);
    scene.wait(1);
    animate(scene, shape).shift(-2*e1);
    scene.wait(1);
    shape->rotate(0.05, e12 + 0.5*e13 + 0.2*e23);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(scene.time_size() - 1, 1, 1) == Color("FFFFFF"));
}

TEST_CASE("Shape set_vertices", "[object]") {
    auto scene = TestScene(2, 1, 4, 2, 1);
    auto shape = make_shape();
    shape->set_visible(true);
    shape->set_vertices(
        {{ 0,  1, 0},
         { 0, -1, 0},
         {-2, -1, 0},
         {-2,  1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(shape);
    scene.frame_advance();
    shape->set_vertices(
        {{2,  1, 0},
         {2, -1, 0},
         {0, -1, 0},
         {0,  1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == "FFFFFF");
    REQUIRE(scene.get_pixel(0, 1, 0) == "000000");
    REQUIRE(scene.get_pixel(1, 0, 0) == "000000");
    REQUIRE(scene.get_pixel(1, 1, 0) == "FFFFFF");
}

TEST_CASE("Empty shapes", "[object]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto shape = make_shape();
    auto shape2 = make_shape();
    shape2->set_vertices({}, {});
    shape->set_visible(true);
    shape2->set_visible(true);
    scene.add(shape, shape2);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("000000"));
}

TEST_CASE("Shape bounding box", "[object]") {
    using namespace pga3;
    auto shape1 = Shape();
    auto shape2 = Shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape3 = Shape(
        {{ 1,  1, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         { 0,  0, 0},
         {-2,  2, 0}},
        {0, 1, 3, 1, 2, 3, 3, 4, 0}
    );
    REQUIRE_NOTHROW(shape1.get_true_bounding_box());
    auto box2 = shape2.get_true_bounding_box();
    auto box3 = shape3.get_true_bounding_box();
    auto box2p1 = box2.get_inner_lower_left().undual();
    auto box2p2 = box2.get_outer_upper_right().undual();
    auto box3p1 = box3.get_inner_lower_left().undual();
    auto box3p2 = box3.get_outer_upper_right().undual();
    REQUIRE_THAT(box2p1, GAEquals(e0 + -2*e1 - 2*e2));
    REQUIRE_THAT(box2p2, GAEquals(e0 +  2*e1 + 2*e2));
    REQUIRE_THAT(box3p1, GAEquals(e0 + -2*e1 - 2*e2));
    REQUIRE_THAT(box3p2, GAEquals(e0 +  2*e1 + 2*e2));
}

TEST_CASE("Shape scaling to zero", "[object]") {
    auto scene = TestScene(6, 6, 6, 6, 1);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    scene.add(shape);
    shape->set_outline(Color("FFFFFF"), 5);
    shape->scale(0);
    scene.frame_advance();
    for (int x = 0; x < 6; ++x) {
        for (int y = 0; y < 6; ++y) {
            INFO("x = " << x << ", y = " << y);
            REQUIRE(scene.get_pixel(0, x, y) == Color("000000"));
        }
    }
}


TEST_CASE("Object fixed_in_frame", "[object]") {
    auto scene = TestScene(10, 10, 10, 10, 4);
    auto shape = make_shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
         {0, 1, 2, 0, 2, 3}
    );
    shape->set_visible(true);
    shape->set_fixed_in_frame(true);
    scene.add(shape);
    scene.get_camera()->add_updater([&]{
        scene.get_camera()->rotate(τ/8, pga3::e13);
    });
    scene.wait(2);
    for (int i = 0; i < 8; ++i) {
        INFO("i = " << i);
        REQUIRE(scene.get_pixel(i, 3, 3) == Color("FFFFFF"));
        REQUIRE(scene.get_pixel(i, 3, 6) == Color("FFFFFF"));
        REQUIRE(scene.get_pixel(i, 6, 3) == Color("FFFFFF"));
        REQUIRE(scene.get_pixel(i, 6, 6) == Color("FFFFFF"));
        REQUIRE(scene.get_pixel(i, 2, 3) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 3, 2) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 2, 6) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 3, 7) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 7, 3) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 6, 2) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 7, 6) == Color("000000"));
        REQUIRE(scene.get_pixel(i, 6, 7) == Color("000000"));
    }
}

TEST_CASE("Order-independent Transparency", "[object]") {
    auto scene = TestScene(6, 6, 6, 6, 1);

    scene.set_transparency_layers(2);

    auto square1 = make_shape(
        {{ 2,  2},
         { 2, -2},
         {-2, -2},
         {-2,  2}},
         {0, 1, 2, 0, 2, 3}
    );
    auto square2 = make_shape(
        {{ 2,  2},
         { 2, -2},
         {-2, -2},
         {-2,  2}},
         {0, 1, 2, 0, 2, 3}
    );
    square1->set_color("FF0000");
    square2->set_color("00FF00");
    square1->set_opacity(0.5);
    square2->set_opacity(0.5);
    square1->set_visible(true);
    square2->set_visible(true);
    scene.add(square1, square2);
    square2->rotate(0.1, pga3::e23);
    scene.frame_advance();

    REQUIRE(scene.get_pixel(0, 2, 1) == ApproxColor("408000"));
    REQUIRE(scene.get_pixel(0, 2, 4) == ApproxColor("804000"));
}

TEST_CASE("Shape copying", "[object]") {
    auto scene = TestScene(6, 6, 6, 6, 1);
    auto square1 = make_shape(
        {{ 2,  2},
         { 2, -2},
         {-2, -2},
         {-2,  2}},
         {0, 1, 2, 0, 2, 3}
    );
    auto square2 = square1.copy_object();
    scene.check_draw_equivalent(square1, square2);
    auto square3 = square1.copy_object();
    square3->set_vertices(
        {{ 3,  2},
         { 3, -2},
         {-1, -2},
         {-1,  2}},
        {0, 1, 2, 0, 2, 3}
    );
    square3->shift(-vga2::e1);
    scene.check_draw_equivalent(square1, square3);
    scene.check_draw_equivalent(square2, square3);
}

TEST_CASE("Shape interpolating", "[object]") {
    auto shape1 = make_shape(
        {{ 1,  1},
         { 1, -1},
         {-1, -1},
         {-1,  1}},
         {0, 1, 2, 0, 2, 3}
    );
    auto shape2 = make_shape(
        {{ 2,  2},
         { 2, -2},
         {-2, -2},
         {-2,  2}},
         {0, 1, 2, 0, 2, 3}
    );
    auto shape3 = make_shape(
        {{ 3,  3},
         { 3, -3},
         {-3, -3},
         {-3,  3}},
         {0, 1, 2, 0, 2, 3}
    );
    auto shape4 = make_shape(
        {{ 4,  4},
         { 4, -4},
         {-4, -4},
         {-4,  4}},
         {0, 1, 2, 0, 2, 3}
    );
    auto shape5 = make_shape(
        {{ 5,  5},
         { 5, -5},
         {-5, -5},
         {-5,  5}},
         {0, 1, 2, 0, 2, 3}
    );
    auto test = shape1.copy_object();
    auto scene = TestScene(12, 12, 12, 12, 1);

    test->interpolate(*shape1, *shape5, 0);
    scene.check_draw_equivalent(test, shape1, "0");

    test->interpolate(*shape1, *shape5, 0.25);
    scene.check_draw_equivalent(test, shape2, "0.25");

    test->interpolate(*shape1, *shape5, 0.5);
    scene.check_draw_equivalent(test, shape3, "0.5");

    test->interpolate(*shape1, *shape5, 0.75);
    scene.check_draw_equivalent(test, shape4, "0.75");

    test->interpolate(*shape1, *shape5, 1);
    scene.check_draw_equivalent(test, shape5, "1");
}

TEST_CASE("Shape outline over other object", "[object]") {
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto big_shape = make_shape(
        {{ 5,  5},
         { 5, -5},
         {-5, -5},
         {-5,  5}},
         {0, 1, 2, 0, 2, 3}
    );
    auto small_shape = make_shape(
        {{ 1,  1},
         { 1, -1},
         {-1, -1},
         {-1,  1}},
         {0, 1, 2, 0, 2, 3}
    );
    small_shape->set_color("00FF00");
    small_shape->set_outline("FF0000", 2);
    big_shape->set_visible(true);
    small_shape->set_visible(true);
    scene.add(big_shape, small_shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("FF0000"));
}
