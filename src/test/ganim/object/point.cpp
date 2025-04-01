#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/point.hpp"

using namespace ganim;

TEST_CASE("Point", "[object]") {
    using namespace vga2;
    auto point = make_point(e1, 2);
    auto scene = TestScene(8, 8, 8, 8, 1);
    auto red = Color("FF0000");
    auto black = Color("000000");
    point->set_color(red);
    point->set_visible(true);
    scene.add(point);
    scene.frame_advance();
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x: " << x << " y: " << y);
            if (x >= 4 and x <= 5 and y >= 3 and y <= 4) {
                REQUIRE(scene.get_pixel(0, x, y) == red);
            }
            else if (x >= 3 and x <= 6 and y >= 2 and y <= 5) {
                auto color = scene.get_pixel(0, x, y);
                REQUIRE(color != black);
            }
            else {
                REQUIRE(scene.get_pixel(0, x, y) == black);
            }
        }
    }
}
