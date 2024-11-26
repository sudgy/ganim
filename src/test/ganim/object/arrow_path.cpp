#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/arrow_path.hpp"

#include "ganim/object/polygon_shape.hpp"
#include "ganim/math.hpp"

using namespace ganim;
using namespace vga2;

TEST_CASE("ArrowPath straight line", "[object]") {
    auto scene = TestScene(6, 6, 6, 6, 1);

    auto path = ArrowPath(
        {
            -2*e1,
            2*e1
        },
        {2, 2}
    );
    path.set_visible(true);

    auto shape1 = PolygonShape(
        {
            -2*e1 + e2,
            -2*e1 - e2,
            -e2,
            e2
        }
    );
    auto shape2 = PolygonShape(
        {
            e2,
            -e2,
            2*e1
        }
    );
    auto group = Group(shape1, shape2);
    group.set_visible(true);

    scene.check_draw_equivalent(path, group);
}

TEST_CASE("ArrowPath straight left line", "[object]") {
    auto scene = TestScene(6, 6, 6, 6, 1);

    auto path = ArrowPath(
        {
            2*e1,
            -2*e1
        },
        {2, 2}
    );
    path.set_visible(true);

    auto shape1 = PolygonShape(
        {
            -2*e1 + e2,
            -2*e1 - e2,
            -e2,
            e2
        }
    );
    auto shape2 = PolygonShape(
        {
            e2,
            -e2,
            2*e1
        }
    );
    auto group = Group(shape1, shape2);
    group.rotate(τ/2);
    group.set_visible(true);

    scene.check_draw_equivalent(path, group);
}

TEST_CASE("ArrowPath turning", "[object]") {
    auto scene = TestScene(10, 10, 10, 10, 1);

    auto path = ArrowPath(
        {
            -2*e1,
            2*e1,
            2*e1 + 4*e2
        },
        {2, 2}
    );
    path.set_visible(true);

    auto tipless_path = Path(
        {
            -2*e1,
            2*e1,
            2*e1 + 2*e2
        },
        false,
        2
    );
    auto tip = PolygonShape(
        {
            e1 + 2*e2,
            3*e1 + 2*e2,
            2*e1 + 4*e2
        }
    );
    auto group = Group(tipless_path, tip);
    group.set_visible(true);

    scene.check_draw_equivalent(path, group);
}

TEST_CASE("ArrowPath turning right at the end", "[object]") {
    auto scene = TestScene(6, 6, 6, 6, 1);

    auto path = ArrowPath(
        {
            -2*e1,
            2*e1 + 0.5*e2,
            2*e1
        },
        {2, 2}
    );
    path.set_visible(true);

    auto shape1 = PolygonShape(
        {
            -2*e1 + e2,
            -2*e1 - e2,
            -e2,
            e2
        }
    );
    auto shape2 = PolygonShape(
        {
            e2,
            -e2,
            2*e1
        }
    );
    auto group = Group(shape1, shape2);
    group.set_visible(true);

    scene.check_draw_equivalent(path, group);
}
