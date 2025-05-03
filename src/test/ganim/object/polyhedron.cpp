#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/polyhedron.hpp"

#include "ganim/object/polygon_shape.hpp"
#include "ganim/object/path.hpp"

using namespace ganim;

TEST_CASE("Polyhedron", "[object]") {
    using namespace vga3;
    auto points = std::vector{-e1 - e2, e1 - e2, e2, e1 + e2 + e3};
    auto polyhedron = make_polyhedron(
        points,
        {
            {0, 2, 1},
            {0, 1, 3},
            {0, 3, 2},
            {1, 2, 3}
        }
    );
    auto face1 = make_polygon_shape({points[0], points[2], points[1]});
    auto face2 = make_polygon_shape({points[0], points[1], points[3]});
    auto face3 = make_polygon_shape({points[0], points[3], points[2]});
    auto face4 = make_polygon_shape({points[1], points[2], points[3]});
    face1->do_shading(true);
    face1->set_color("FFFFFF9F");
    face2->set_color("FFFFFF9F");
    face3->set_color("FFFFFF9F");
    face4->set_color("FFFFFF9F");
    auto edge1 = make_path({points[0], points[1]}, 8);
    auto edge2 = make_path({points[0], points[2]}, 8);
    auto edge3 = make_path({points[0], points[3]}, 8);
    auto edge4 = make_path({points[1], points[2]}, 8);
    auto edge5 = make_path({points[1], points[3]}, 8);
    auto edge6 = make_path({points[2], points[3]}, 8);
    auto group = make_group(face1, face2, face3, face4, edge1, edge2, edge3,
            edge4, edge5, edge6);

    auto scene = TestScene(32, 32, 4, 4, 1);
    scene.set_transparency_layers(16);
    scene.check_draw_equivalent(polyhedron, group, "", "test");
}
