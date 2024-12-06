#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/bivector.hpp"

#include "ganim/object/vector.hpp"

using namespace ganim;

TEST_CASE("Bivector pga list constructor", "[object]") {
    using namespace pga2;
    auto points = std::vector{
        (e0 - 2*e1 - 2*e2).dual(),
        (e0 + 2*e1 - 2*e2).dual(),
        (e0 + 2*e1 + 2*e2).dual()
    };
    auto bivector = Bivector(points, {0.6, 2, 2});
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 3);

    auto inside = PolygonShape(points);
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({points[0], points[1]}, {2, 2});
    auto outside2 = ArrowPath({points[1], points[2]}, {2, 2});
    auto outside3 = ArrowPath({points[2], points[0]}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3);
    group.set_visible(true);

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
    auto bivector = Bivector(points, {0.6, 2, 2});
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 3);

    auto inside = PolygonShape(points);
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({points[0], points[1]}, {2, 2});
    auto outside2 = ArrowPath({points[1], points[2]}, {2, 2});
    auto outside3 = ArrowPath({points[2], points[0]}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3);
    group.set_visible(true);

    auto scene = TestScene(6, 6, 6, 6, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector Vector constructor", "[object]") {
    using namespace vga2;
    auto obj1 = Vector(4*e1);
    auto obj2 = Vector(4*e2);
    obj1.shift(-e1);
    obj2.shift(e1 + e2);
    auto bivector = Bivector(obj1, obj2, {0.6, 2, 2});
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 4);

    auto inside = PolygonShape({0*e1, 4*e1, 4*e1 + 4*e2, 4*e2});
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({0*e1, 4*e1}, {2, 2});
    auto outside2 = ArrowPath({4*e1, 4*e1 + 4*e2}, {2, 2});
    auto outside3 = ArrowPath({4*e1 + 4*e2, 4*e2}, {2, 2});
    auto outside4 = ArrowPath({4*e2, 0*e1}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    outside4.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3, outside4);
    group.set_visible(true);
    group.shift(-e1);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 2D VGA constructor", "[object]") {
    using namespace vga2;
    auto bivector = Bivector(
        4*e1,
        4*e2,
        {0.6, 2, 2}
    );
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 4);

    auto inside = PolygonShape({0*e1, 4*e1, 4*e1 + 4*e2, 4*e2});
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({0*e1, 4*e1}, {2, 2});
    auto outside2 = ArrowPath({4*e1, 4*e1 + 4*e2}, {2, 2});
    auto outside3 = ArrowPath({4*e1 + 4*e2, 4*e2}, {2, 2});
    auto outside4 = ArrowPath({4*e2, 0*e1}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    outside4.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3, outside4);
    group.set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 3D VGA constructor", "[object]") {
    auto bivector = Bivector(
        4*vga3::e1,
        4*vga3::e2 + 3*vga3::e3,
        {0.6, 2, 2}
    );
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 4);

    using namespace vga2;
    auto inside = PolygonShape({0*e1, 4*e1, 4*e1 + 5*e2, 5*e2});
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({0*e1, 4*e1}, {2, 2});
    auto outside2 = ArrowPath({4*e1, 4*e1 + 5*e2}, {2, 2});
    auto outside3 = ArrowPath({4*e1 + 5*e2, 5*e2}, {2, 2});
    auto outside4 = ArrowPath({5*e2, 0*e1}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    outside4.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3, outside4);
    group.rotate(std::atan2(3, 4), vga3::e23);
    group.set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 2D VGA bivector constructor", "[object]") {
    using namespace vga2;
    auto bivector = Bivector(
        16*e12,
        {0.6, 2, 2}
    );
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 4);

    auto inside = PolygonShape({0*e1, 4*e1, 4*e1 + 4*e2, 4*e2});
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({0*e1, 4*e1}, {2, 2});
    auto outside2 = ArrowPath({4*e1, 4*e1 + 4*e2}, {2, 2});
    auto outside3 = ArrowPath({4*e1 + 4*e2, 4*e2}, {2, 2});
    auto outside4 = ArrowPath({4*e2, 0*e1}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    outside4.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3, outside4);
    group.set_visible(true);

    auto scene = TestScene(10, 10, 10, 10, 1);
    scene.check_draw_equivalent(bivector, group);
}

TEST_CASE("Bivector 3D VGA bivector constructor", "[object]") {
    auto bivector = Bivector(
        (5*vga3::e1) ^ (4*vga3::e2 + 3*vga3::e3),
        {0.6, 2, 2}
    );
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 4);

    using namespace vga2;
    auto inside = PolygonShape({0*e1, 5*e1, 5*e1 + 5*e2, 5*e2});
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({0*e1, 5*e1}, {2, 2});
    auto outside2 = ArrowPath({5*e1, 5*e1 + 5*e2}, {2, 2});
    auto outside3 = ArrowPath({5*e1 + 5*e2, 5*e2}, {2, 2});
    auto outside4 = ArrowPath({5*e2, 0*e1}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    outside3.set_color(Color("FFFF00"));
    outside4.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2, outside3, outside4);
    group.rotate(std::atan2(3, 4), vga3::e23);
    group.set_visible(true);

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
    auto bivector = Bivector(points, {0.6, 2, 2});
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 2);

    auto inside = PolygonShape(points);
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({points[0], points[1], points[2]}, {2, 2});
    auto outside2 = ArrowPath({points[2], points[0]}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2);
    group.set_visible(true);

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
    auto bivector = Bivector(points, {0.6, 2, 2});
    bivector.set_color(Color("FFFF00"));
    bivector.set_visible(true);
    REQUIRE(bivector.get_outside().size() == 2);

    auto inside = PolygonShape(points);
    inside.set_color(Color("FFFF00"));
    inside.set_opacity(0.5);
    auto outside1 = ArrowPath({points[0], points[1]}, {2, 2});
    auto outside2 = ArrowPath({points[1], points[2], points[0]}, {2, 2});
    outside1.set_color(Color("FFFF00"));
    outside2.set_color(Color("FFFF00"));
    auto group = Group(inside, outside1, outside2);
    group.set_visible(true);

    auto scene = TestScene(6, 6, 6, 6, 1);
    scene.check_draw_equivalent(bivector, group);
}
