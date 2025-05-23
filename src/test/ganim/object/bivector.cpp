#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/bivector.hpp"

#include "ganim/object/vector.hpp"
#include "ganim/animation/fading.hpp"

using namespace ganim;

TEST_CASE("Bivector pga list constructor", "[object]") {
    using namespace pga2;
    auto points = std::vector{
        (e0 - 2*e1 - 2*e2).dual(),
        (e0 + 2*e1 - 2*e2).dual(),
        (e0 + 2*e1 + 2*e2).dual()
    };
    auto bivector = make_bivector(points, {0.6, 2, 2});
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 3);

    auto inside = make_polygon_shape(points);
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({points[0], points[1]}, {2, 2});
    auto outside2 = make_arrow_path({points[1], points[2]}, {2, 2});
    auto outside3 = make_arrow_path({points[2], points[0]}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3);
    group->set_visible(true);

    auto scene = TestScene(6, 6, 6, 6, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector vga list constructor", "[object]") {
    using namespace vga2;
    auto points = std::vector{
        -2*e1 - 2*e2,
         2*e1 - 2*e2,
         2*e1 + 2*e2
    };
    auto bivector = make_bivector(points, {0.6, 2, 2});
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 3);

    auto inside = make_polygon_shape(points);
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({points[0], points[1]}, {2, 2});
    auto outside2 = make_arrow_path({points[1], points[2]}, {2, 2});
    auto outside3 = make_arrow_path({points[2], points[0]}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3);
    group->set_visible(true);

    auto scene = TestScene(6, 6, 6, 6, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector Vector constructor", "[object]") {
    using namespace vga2;
    auto obj1 = Vector(4*e1);
    auto obj2 = Vector(4*e2);
    obj1.shift(-e1);
    obj2.shift(e1 + e2);
    auto bivector = make_bivector(
            obj1, obj2, {0.6, 2, 2});
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 4);

    auto inside = make_polygon_shape({0*e1, 4*e1, 4*e1 + 4*e2, 4*e2});
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({0*e1, 4*e1}, {2, 2});
    auto outside2 = make_arrow_path({4*e1, 4*e1 + 4*e2}, {2, 2});
    auto outside3 = make_arrow_path({4*e1 + 4*e2, 4*e2}, {2, 2});
    auto outside4 = make_arrow_path({4*e2, 0*e1}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    outside4->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3, outside4);
    group->set_visible(true);
    group->shift(-e1);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 2D VGA constructor", "[object]") {
    using namespace vga2;
    auto bivector = make_bivector(
        4*e1,
        4*e2,
        {0.6, 2, 2}
    );
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 4);

    auto inside = make_polygon_shape({0*e1, 4*e1, 4*e1 + 4*e2, 4*e2});
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({0*e1, 4*e1}, {2, 2});
    auto outside2 = make_arrow_path({4*e1, 4*e1 + 4*e2}, {2, 2});
    auto outside3 = make_arrow_path({4*e1 + 4*e2, 4*e2}, {2, 2});
    auto outside4 = make_arrow_path({4*e2, 0*e1}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    outside4->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3, outside4);
    group->set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 3D VGA constructor", "[object]") {
    auto bivector = make_bivector(
        4*vga3::e1,
        4*vga3::e2 + 3*vga3::e3,
        {0.6, 2, 2}
    );
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 4);

    using namespace vga2;
    auto inside = make_polygon_shape({0*e1, 4*e1, 4*e1 + 5*e2, 5*e2});
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({0*e1, 4*e1}, {2, 2});
    auto outside2 = make_arrow_path({4*e1, 4*e1 + 5*e2}, {2, 2});
    auto outside3 = make_arrow_path({4*e1 + 5*e2, 5*e2}, {2, 2});
    auto outside4 = make_arrow_path({5*e2, 0*e1}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    outside4->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3, outside4);
    group->rotate(std::atan2(3, 4), vga3::e23);
    group->set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 2D VGA bivector constructor", "[object]") {
    using namespace vga2;
    auto bivector = make_bivector(
        16*e12,
        {0.6, 2, 2}
    );
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 4);

    auto inside = make_polygon_shape({0*e1, 4*e1, 4*e1 + 4*e2, 4*e2});
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({0*e1, 4*e1}, {2, 2});
    auto outside2 = make_arrow_path({4*e1, 4*e1 + 4*e2}, {2, 2});
    auto outside3 = make_arrow_path({4*e1 + 4*e2, 4*e2}, {2, 2});
    auto outside4 = make_arrow_path({4*e2, 0*e1}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    outside4->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3, outside4);
    group->set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 3D VGA bivector constructor", "[object]") {
    auto bivector = make_bivector(
        (5*vga3::e1) ^ (4*vga3::e2 + 3*vga3::e3),
        {0.6, 2, 2}
    );
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 4);

    using namespace vga2;
    auto inside = make_polygon_shape({0*e1, 5*e1, 5*e1 + 5*e2, 5*e2});
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({0*e1, 5*e1}, {2, 2});
    auto outside2 = make_arrow_path({5*e1, 5*e1 + 5*e2}, {2, 2});
    auto outside3 = make_arrow_path({5*e1 + 5*e2, 5*e2}, {2, 2});
    auto outside4 = make_arrow_path({5*e2, 0*e1}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    outside3->set_color(Color("FFFF00"));
    outside4->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2, outside3, outside4);
    group->rotate(std::atan2(3, 4), vga3::e23);
    group->set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector small change at start", "[object]") {
    using namespace vga2;
    auto points = std::vector{
        1.9*e1 - 2*e2,
          2*e1 - 2*e2,
          2*e1 + 2*e2
    };
    auto bivector = make_bivector(points, {0.6, 2, 2});
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 2);

    auto inside = make_polygon_shape(points);
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({points[0], points[1], points[2]}, {2, 2});
    auto outside2 = make_arrow_path({points[2], points[0]}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2);
    group->set_visible(true);

    auto scene = TestScene(6, 6, 6, 6, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector small change in the middle", "[object]") {
    using namespace vga2;
    auto points = std::vector{
        -2*e1 - 2*e2,
         2*e1 - 2*e2,
         2*e1 - 1.9*e2
    };
    auto bivector = make_bivector(points, {0.6, 2, 2});
    bivector->set_color(Color("FFFF00"));
    bivector->set_visible(true);
    REQUIRE(bivector->get_outside()->size() == 2);

    auto inside = make_polygon_shape(points);
    inside->set_color(Color("FFFF00"));
    inside->set_opacity(0.5);
    auto outside1 = make_arrow_path({points[0], points[1]}, {2, 2});
    auto outside2 = make_arrow_path({points[1], points[2], points[0]}, {2, 2});
    outside1->set_color(Color("FFFF00"));
    outside2->set_color(Color("FFFF00"));
    auto group = make_group(inside, outside1, outside2);
    group->set_visible(true);

    auto scene = TestScene(6, 6, 6, 6, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 2D negative orientation", "[object]") {
    using namespace vga2;
    auto b1 = make_bivector(-e12);
    auto b2 = make_bivector(e2, e1);
    auto scene = TestScene(32, 32, 4, 4, 1);
    scene.check_draw_equivalent(b1, b2);
}

TEST_CASE("Bivector e21 in 3D", "[object]") {
    using namespace vga3;
    auto b1 = make_bivector(-e12);
    auto b2 = make_bivector(e2, e1);
    auto scene = TestScene(32, 32, 4, 4, 1);
    scene.check_draw_equivalent(b1, b2);
}

TEST_CASE("Bivector color alpha", "[object]") {
    using namespace vga2;
    auto b = make_bivector(e1, e2);
    REQUIRE(b->get_outside()->get_color() == "FFFFFF");
    REQUIRE(b->get_inside()->get_color() == "FFFFFF7F");
    b->set_color("FF0000");
    REQUIRE(b->get_outside()->get_color() == "FF0000");
    REQUIRE(b->get_inside()->get_color() == "FF00007F");
}

TEST_CASE("Bivector polymorphic_copy", "[object]") {
    using namespace vga2;
    auto b1 = make_bivector(e1, e2);
    b1->shift(e1);
    b1->set_color("FF0000");
    auto b2 = b1->polymorphic_copy();
    b2->set_color(b2->get_color());
    auto scene = TestScene(32, 32, 4, 4, 1);
    scene.check_draw_equivalent(b1, b2);
}

TEST_CASE("Bivector 3D list of points", "[object]") {
    using namespace vga3;
    auto scene = TestScene(32, 32, 4, 4, 1);
    auto b1 = make_bivector({
        -3*e1 - 3*e2 - 2*e3,
         3*e1 - 3*e2 - 2*e3,
         3*e1 + 3*e2 + 2*e3,
        -3*e1 + 3*e2 + 2*e3
    });
    auto b2 = make_bivector(6*e1, 6*e2 + 4*e3);
    b2->shift(-3*e1 - 3*e2 - 2*e3);
    scene.check_draw_equivalent(b1, b2);
}

TEST_CASE("Bivector fade_out with changed subobject", "[object]") {
    auto test = make_bivector(vga2::e12);
    auto white = Color("FFFFFF");
    auto red = Color("FF0000");
    test->set_color(white);
    auto a = test->get_outside()[0];
    a->set_color(red);
    auto scene = TestScene(1, 1, 1, 1, 4);
    fade_out(scene, test);
    scene.wait();
    REQUIRE(a->get_color() == red);
}
