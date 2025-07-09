#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/scene_object.hpp"
#include "ganim/object/polygon_shape.hpp"

using namespace ganim;

TEST_CASE("SceneObject", "[object]") {
    using namespace vga2;
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto scene2 = make_scene_object(2, 2, 2, 2, 1);
    auto rect = make_polygon_shape({-e1, e1, e1 + e2, -e1 + e2});
    int updated = 0;
    rect->add_updater([&]{++updated;});
    scene2->set_background_color("80808080");
    scene2->set_visible(true);
    rect->set_visible(true);
    scene2->add(rect);
    scene.add(scene2);
    scene.frame_advance();
    REQUIRE(updated == 1);
    scene2->shift(e1);
    scene.frame_advance();
    REQUIRE(updated == 2);
    rect->shift(-e1 - e2);
    scene.frame_advance();
    REQUIRE(updated == 3);

    const auto black = Color("000000");
    const auto white = Color("FFFFFF");
    const auto gray = Color("404040");
    REQUIRE(scene.get_pixel(0, 0, 0) == black);
    REQUIRE(scene.get_pixel(0, 1, 0) == black);
    REQUIRE(scene.get_pixel(0, 2, 0) == black);
    REQUIRE(scene.get_pixel(0, 3, 0) == black);
    REQUIRE(scene.get_pixel(0, 0, 1) == black);
    REQUIRE(scene.get_pixel(0, 1, 1) == white);
    REQUIRE(scene.get_pixel(0, 2, 1) == white);
    REQUIRE(scene.get_pixel(0, 3, 1) == black);
    REQUIRE(scene.get_pixel(0, 0, 2) == black);
    REQUIRE(scene.get_pixel(0, 1, 2) == gray);
    REQUIRE(scene.get_pixel(0, 2, 2) == gray);
    REQUIRE(scene.get_pixel(0, 3, 2) == black);
    REQUIRE(scene.get_pixel(0, 0, 3) == black);
    REQUIRE(scene.get_pixel(0, 1, 3) == black);
    REQUIRE(scene.get_pixel(0, 2, 3) == black);
    REQUIRE(scene.get_pixel(0, 3, 3) == black);

    REQUIRE(scene.get_pixel(1, 0, 0) == black);
    REQUIRE(scene.get_pixel(1, 1, 0) == black);
    REQUIRE(scene.get_pixel(1, 2, 0) == black);
    REQUIRE(scene.get_pixel(1, 3, 0) == black);
    REQUIRE(scene.get_pixel(1, 0, 1) == black);
    REQUIRE(scene.get_pixel(1, 1, 1) == black);
    REQUIRE(scene.get_pixel(1, 2, 1) == white);
    REQUIRE(scene.get_pixel(1, 3, 1) == white);
    REQUIRE(scene.get_pixel(1, 0, 2) == black);
    REQUIRE(scene.get_pixel(1, 1, 2) == black);
    REQUIRE(scene.get_pixel(1, 2, 2) == gray);
    REQUIRE(scene.get_pixel(1, 3, 2) == gray);
    REQUIRE(scene.get_pixel(1, 0, 3) == black);
    REQUIRE(scene.get_pixel(1, 1, 3) == black);
    REQUIRE(scene.get_pixel(1, 2, 3) == black);
    REQUIRE(scene.get_pixel(1, 3, 3) == black);

    REQUIRE(scene.get_pixel(2, 0, 0) == black);
    REQUIRE(scene.get_pixel(2, 1, 0) == black);
    REQUIRE(scene.get_pixel(2, 2, 0) == black);
    REQUIRE(scene.get_pixel(2, 3, 0) == black);
    REQUIRE(scene.get_pixel(2, 0, 1) == black);
    REQUIRE(scene.get_pixel(2, 1, 1) == black);
    REQUIRE(scene.get_pixel(2, 2, 1) == gray);
    REQUIRE(scene.get_pixel(2, 3, 1) == gray);
    REQUIRE(scene.get_pixel(2, 0, 2) == black);
    REQUIRE(scene.get_pixel(2, 1, 2) == black);
    REQUIRE(scene.get_pixel(2, 2, 2) == white);
    REQUIRE(scene.get_pixel(2, 3, 2) == gray);
    REQUIRE(scene.get_pixel(2, 0, 3) == black);
    REQUIRE(scene.get_pixel(2, 1, 3) == black);
    REQUIRE(scene.get_pixel(2, 2, 3) == black);
    REQUIRE(scene.get_pixel(2, 3, 3) == black);
}
