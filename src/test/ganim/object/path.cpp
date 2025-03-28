#include <catch2/catch_test_macros.hpp>

#include "ganim/object/path.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("Path straight line", "[object]") {
    using namespace vga2;
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto path = make_path(
        {
            -2*e1,
            2*e1
        },
        false,
        2
    );
    path->set_visible(true);
    scene.add(path);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x >= 2 and x <= 5 and y >= 3 and y <= 4) color=Color("FFFFFF");
            REQUIRE(scene.get_pixel(0, x, y) == color);
        }
    }
}

// In this test, it only tests for points near the path because I'm not 100%
// certain how exactly the elbow will affect the pixels on the line of the
// elbow.  If you find a weird issue with points not on the curve, add another
// test.
TEST_CASE("Path single turn", "[object]") {
    using namespace vga2;
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto path1 = make_path(
        {
            -2*e1,
            0*e1,
            2*e1
        },
        false,
        2
    );
    path1->set_visible(true);
    scene.add(path1);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x >= 2 and x <= 5 and y >= 3 and y <= 4) color=Color("FFFFFF");
            REQUIRE(scene.get_pixel(0, x, y) == color);
        }
    }
    auto path2 = make_path(
        {
            -2*e1,
            2*e1,
            0*e1
        },
        false,
        2
    );
    path2->set_visible(true);
    path1.reset();
    scene.add(path2);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("000000");
            if (x >= 2 and x <= 5 and y >= 3 and y <= 4) color=Color("FFFFFF");
            REQUIRE(scene.get_pixel(1, x, y) == color);
        }
    }
    auto path3 = make_path(
        {
            -4*e1,
            2*e1,
            6*e2 - e1
        },
        false,
        4
    );
    path3->set_visible(true);
    path2.reset();
    scene.add(path3);
    scene.frame_advance();
    auto white = Color("FFFFFF");
    for (int x = 0; x < 7; ++x) {
        for (int y = 3; y < 5; ++y) {
            INFO("x = " << x << ", y = " << y);
            REQUIRE(scene.get_pixel(2, x, y) == white);
            REQUIRE(scene.get_pixel(2, x, y) == white);
        }
    }
    REQUIRE(scene.get_pixel(2, 4, 2) == white);
    REQUIRE(scene.get_pixel(2, 5, 2) == white);
    REQUIRE(scene.get_pixel(2, 4, 1) == white);
    REQUIRE(scene.get_pixel(2, 5, 1) == white);
    REQUIRE(scene.get_pixel(2, 3, 0) == white);
    REQUIRE(scene.get_pixel(2, 4, 0) == white);
    auto path4 = make_path(
        {
            -4*e1,
            0*e1,
            4*e2
        },
        false,
        4
    );
    path4->set_visible(true);
    path3.reset();
    scene.add(path4);
    scene.frame_advance();
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = white;
            if (x < 2 and y < 2) color = Color("000000");
            REQUIRE(scene.get_pixel(3, x, y) == color);
            REQUIRE(scene.get_pixel(3, x, y) == color);
        }
    }
    auto path5 = make_path(
        {
            -4*e1,
            -2*e1,
            6*e2 + e1
        },
        false,
        4
    );
    path5->set_visible(true);
    path4.reset();
    scene.add(path5);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(4, 0, 3) == white);
    REQUIRE(scene.get_pixel(4, 0, 4) == white);
    REQUIRE(scene.get_pixel(4, 1, 3) == white);
    REQUIRE(scene.get_pixel(4, 1, 4) == white);
    REQUIRE(scene.get_pixel(4, 2, 3) == white);
    REQUIRE(scene.get_pixel(4, 2, 4) == white);
    REQUIRE(scene.get_pixel(4, 2, 1) == white);
    REQUIRE(scene.get_pixel(4, 2, 2) == white);
    REQUIRE(scene.get_pixel(4, 3, 1) == white);
    REQUIRE(scene.get_pixel(4, 3, 2) == white);
    REQUIRE(scene.get_pixel(4, 3, 0) == white);
    REQUIRE(scene.get_pixel(4, 4, 0) == white);
    auto path6 = make_path(
        {
            -4*e1,
            2*e1,
            -6*e2 - e1
        },
        false,
        4
    );
    path6->set_visible(true);
    path5.reset();
    scene.add(path6);
    scene.frame_advance();
    for (int x = 0; x < 7; ++x) {
        for (int y = 3; y < 5; ++y) {
            INFO("x = " << x << ", y = " << y);
            REQUIRE(scene.get_pixel(5, x, y) == white);
            REQUIRE(scene.get_pixel(5, x, y) == white);
        }
    }
    REQUIRE(scene.get_pixel(5, 4, 5) == white);
    REQUIRE(scene.get_pixel(5, 5, 5) == white);
    REQUIRE(scene.get_pixel(5, 4, 6) == white);
    REQUIRE(scene.get_pixel(5, 5, 6) == white);
    REQUIRE(scene.get_pixel(5, 3, 7) == white);
    REQUIRE(scene.get_pixel(5, 4, 7) == white);
    auto path7 = make_path(
        {
            -4*e1,
            0*e1,
            -4*e2
        },
        false,
        4
    );
    path7->set_visible(true);
    path6.reset();
    scene.add(path7);
    scene.frame_advance();
    for (int x = 0; x < 5; ++x) {
        for (int y = 3; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = white;
            if (x < 2 and y > 5) color = Color("000000");
            REQUIRE(scene.get_pixel(6, x, y) == color);
            REQUIRE(scene.get_pixel(6, x, y) == color);
        }
    }
    auto path8 = make_path(
        {
            -4*e1,
            -2*e1,
            -6*e2 + e1
        },
        false,
        4
    );
    path8->set_visible(true);
    path7.reset();
    scene.add(path8);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(7, 0, 3) == white);
    REQUIRE(scene.get_pixel(7, 0, 4) == white);
    REQUIRE(scene.get_pixel(7, 1, 3) == white);
    REQUIRE(scene.get_pixel(7, 1, 4) == white);
    REQUIRE(scene.get_pixel(7, 2, 3) == white);
    REQUIRE(scene.get_pixel(7, 2, 4) == white);
    REQUIRE(scene.get_pixel(7, 2, 5) == white);
    REQUIRE(scene.get_pixel(7, 2, 6) == white);
    REQUIRE(scene.get_pixel(7, 3, 5) == white);
    REQUIRE(scene.get_pixel(7, 3, 6) == white);
    REQUIRE(scene.get_pixel(7, 3, 7) == white);
    REQUIRE(scene.get_pixel(7, 4, 7) == white);
}

TEST_CASE("Path elbows", "[object]") {
    using namespace vga2;
    auto scene = TestScene(10, 10, 10, 10, 1);
    auto path = make_path(
        {
            -4*e1,
            0*e1,
            4*e2
        },
        false,
        8
    );
    path->set_visible(true);
    scene.add(path);
    scene.frame_advance();
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("FFFFFF");
            if (x == 0 or x == 9 or y == 0 or y == 9) color = Color("000000");
            // Along the boundary of the elbow, don't know if this is
            // GPU-dependent so don't bother checking
            if ((x == 5 and y == 8) or
                (x == 6 and y == 8) or
                (x == 7 and y == 8) or
                (x == 6 and y == 7) or
                (x == 7 and y == 7) or
                (x == 8 and y == 7) or
                (x == 7 and y == 6) or
                (x == 8 and y == 6) or
                (x == 8 and y == 5)) continue;
            if (x == 8 and y == 8) color = Color("000000");
            REQUIRE(scene.get_pixel(0, x, y) == color);
        }
    }
}

TEST_CASE("Closed paths", "[object]") {
    using namespace vga2;
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto path = make_path(
        {
            -2*e1 - 2*e2,
            +2*e1 - 2*e2,
            +2*e1 + 2*e2,
            -2*e1 + 2*e2
        },
        true,
        2
    );
    path->set_visible(true);
    scene.add(path);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color = Color("FFFFFF");
            if (x == 0 or x == 7 or y == 0 or y == 7) color = Color("000000");
            if (x >= 3 and x <= 4 and y >= 3 and y <= 4) color=Color("000000");
            if ((x == 1 or x == 6) and (y == 1 or y == 6)) continue;
            REQUIRE(scene.get_pixel(0, x, y) == color);
        }
    }
}

TEST_CASE("Dashed paths", "[object]") {
    using namespace vga2;
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto path = make_path(
        {
            -2*e1,
            2*e1
        },
        false,
        2
    );
    path->set_visible(true);
    path->set_dash(0.25, 0.25);
    scene.add(path);
    scene.frame_advance();
    path->set_dash(0, 0);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto color1 = Color("000000");
            auto color2 = Color("000000");
            if (x >= 2 and x <= 5 and y >= 3 and y <= 4) {
                if (x == 2 or x == 4) color1 = Color("FFFFFF");
                color2 = Color("FFFFFF");
            }
            REQUIRE(scene.get_pixel(0, x, y) == color1);
            REQUIRE(scene.get_pixel(1, x, y) == color2);
        }
    }
}

TEST_CASE("3D Open Path", "[object]") {
    auto scene = TestScene(32, 32, 4, 4, 1);
    using namespace vga3;
    auto path = make_path(
        {
            -e1 - e2,
             e1 - e2,
             e1 + e2,
            -e1 + e2,
        },
        8,
        false,
        1
    );
    path->shift(-e1);
    path->do_shading(false);
    auto shape = make_shape(
        {
            {-1, -1 + 0.5},
            {-1, -1 - 0.5},
            {1 - 0.5, -1 + 0.5},
            {1 + 0.5, -1 - 0.5},
            {1 - 0.5,  1 - 0.5},
            {1 + 0.5,  1 + 0.5},
            {-1, 1 - 0.5},
            {-1, 1 + 0.5},
        },
        {0, 1, 2, 2, 1, 3, 2, 3, 5, 5, 4, 2, 5, 7, 6, 5, 6, 4}
    );
    shape->shift(-e1);
    scene.check_draw_equivalent(path, shape);
}

TEST_CASE("3D Closed Path", "[object]") {
    auto scene = TestScene(32, 32, 4, 4, 1);
    using namespace vga3;
    auto path = make_path(
        {
            -e1 - e2,
             e1 - e2,
             e1 + e2,
            -e1 + e2,
        },
        8,
        true,
        1
    );
    path->do_shading(false);
    auto shape = make_shape(
        {
            {-1 + 0.5, -1 + 0.5},
            {-1 - 0.5, -1 - 0.5},
            { 1 - 0.5, -1 + 0.5},
            { 1 + 0.5, -1 - 0.5},
            { 1 - 0.5,  1 - 0.5},
            { 1 + 0.5,  1 + 0.5},
            {-1 + 0.5,  1 - 0.5},
            {-1 - 0.5,  1 + 0.5},
        },
        {0, 1, 2, 2, 1, 3, 2, 3, 5, 5, 4, 2, 5, 7, 6, 5, 6, 4, 0, 6, 1, 7, 1, 6}
    );
    scene.check_draw_equivalent(path, shape);
}
