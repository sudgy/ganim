#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ganim/util/triangulation.hpp"

using namespace ganim;
using namespace vga2;

namespace {
    void test_triangulation(
        const std::vector<vga2::Vec>& polygon,
        const std::vector<unsigned>& triangulation,
        const char* description,
        bool degenerate = false,
        bool collinear = false
    )
    {
        INFO(description);
        if (degenerate) {
            REQUIRE(ssize(triangulation) <= (ssize(polygon) - 2) * 3);
        }
        else {
            REQUIRE(ssize(triangulation) == (ssize(polygon) - 2) * 3);
        }
        for (auto i : triangulation) {
            REQUIRE(i < polygon.size());
        }
        if (!collinear) {
            auto edge_counts = std::vector<int>();
            auto vertex_counts = std::vector<int>();
            edge_counts.resize(polygon.size());
            vertex_counts.resize(polygon.size());
            for (int i = 0; i < ssize(triangulation) / 3; ++i) {
                auto i1 = triangulation[3*i+0];
                auto i2 = triangulation[3*i+1];
                auto i3 = triangulation[3*i+2];
                ++vertex_counts[i1];
                ++vertex_counts[i2];
                ++vertex_counts[i3];
                auto check_edge = [&](int i1, int i2){
                    auto m1 = std::min(i1, i2);
                    auto m2 = std::max(i1, i2);
                    if (m2 - m1 == 1) ++edge_counts[m1];
                    if (m1 == 0 and m2 == ssize(polygon) - 1) ++edge_counts[m2];
                };
                check_edge(i1, i2);
                check_edge(i2, i3);
                check_edge(i3, i1);
            }
            for (auto i = 0; i < ssize(vertex_counts); ++i) {
                INFO("Vertex number " << i);
                REQUIRE(vertex_counts[i] > 0);
            }
            if (!degenerate) {
                for (auto i = 0; i < ssize(edge_counts); ++i) {
                    INFO("Edge number " << i);
                    REQUIRE(edge_counts[i] == 1);
                }
            }
        }
        // This is actually twice the area, but since we're only checking for
        // equality we can just calculate twice the area for both of them
        auto polygon_area = 0.0;
        for (int i = 0; i < ssize(polygon); ++i) {
            auto this_part = polygon[i] ^ polygon[(i + 1) % ssize(polygon)];
            polygon_area += this_part.blade_project<e12>();
        }
        auto triangle_area = 0.0;
        for (int i = 0; i < ssize(triangulation) / 3; ++i) {
            INFO("Triangle number " << i);
            auto p1 = polygon[triangulation[i*3+0]];
            auto p2 = polygon[triangulation[i*3+1]];
            auto p3 = polygon[triangulation[i*3+2]];
            auto this_area = ((p2 - p1) ^ (p3 - p1)).blade_project<e12>();
            REQUIRE(this_area != 0);
            REQUIRE((this_area > 0) == (polygon_area > 0));
            triangle_area += this_area;
        }
        // As long as all the input vertices have a simple mantissa and not a
        // wide range of exponents, all these calculations should have stayed
        // exact
        REQUIRE(polygon_area == triangle_area);
    }
}

TEST_CASE("Triangulation", "[util]") {
    using namespace vga2;
    REQUIRE_THROWS(triangulate(std::vector<Vec>{}));
    REQUIRE_THROWS(triangulate(std::vector<Vec>{e1}));
    REQUIRE_THROWS(triangulate(std::vector<Vec>{e1, e2}));
    // Simple triangle
    auto t1 = std::vector<Vec>{
        0*e1,
        e1,
        e1 + e2
    };
    // Simple square
    auto t2 = std::vector<Vec>{
        0*e1,
        e1,
        e1 + e2,
        e2
    };
    // Last one but in reverse
    auto t3 = std::vector<Vec>{
        e2,
        e1 + e2,
        e1,
        0*e1
    };
    // More complicated, concave and has multiple repeated x and y values
    auto t4 = std::vector<Vec>{
        -e1 - e2,
        2*e1 - e2,
        2*e1 + 2*e2,
        e1 + 2*e2,
        e1,
        e2,
        2*e2,
        -e1 + 2*e2
    };
    // A square, but with a point halfway along each edge.
    auto t5 = std::vector<Vec>{
        0*e1,
        1*e1,
        2*e1,
        2*e1 + 1*e2,
        2*e1 + 2*e2,
        1*e1 + 2*e2,
        2*e2,
        1*e2
    };
    // A ridiculous shape.  If this doesn't catch an error, I don't know what
    // will.  This was the result of me drawing in my notebook the most
    // ridiculous shape I could think of.
    auto t6 = std::vector<Vec>{
        0*e1,
        -1.5*e1,
        -e1 - e2,
        -2*e1 - e2,
        -2*e1,
        0.5*e2,
        -3*e1 + e2,
        -3.5*e1 - e2,
        -3*e1,
        -3*e1 - 2*e2,
        -e1 - 3*e2,
        -2*e1 - 4*e2,
        -e1 - 4*e2,
        -2*e2,
        -e1 - 2*e2,
        2*e1 - e2,
        2*e1 - 3*e2,
        e1 - 2*e2,
        e1 - 4*e2,
        2*e1 - 3.5*e2,
        3*e1 - 4*e2,
        3*e1 - 3*e2,
        2.5*e1 - 3.5*e2,
        3*e1 - 2*e2,
        2.5*e1 - 2.5*e2,
        3*e1 - e2,
        2.5*e1 - 1.5*e2,
        3*e1,
        2*e1 - 0.5*e2,
        1.5*e1,
        2*e1 + 0.5*e2,
        3*e1 + e2,
        4*e1 + 3*e2,
        3*e1 + 4*e2,
        2.5*e1 + 3.5*e2,
        2*e1 + 4*e2,
        2*e1 + 3*e2,
        3*e1 + 3*e2,
        2*e1 + 2*e2,
        0.5*e1 + 2*e2,
        e1 + 4*e2,
        -3*e1 + 4*e2,
        -3*e1 + 2.5*e2,
        3.5*e2,
        2*e2,
        -0.5*e1 + 2*e2,
        -e1 + 3*e2,
        -2*e1 + 2.5*e2,
        -1.5*e1 + 2*e2,
        -2*e1 + e2,
        e1 + e2,
        -e2,
        -0.5*e1 - e2
    };
    auto t1_concave = triangulate_concave(t1);
    auto t2_concave = triangulate_concave(t2);
    auto t3_concave = triangulate_concave(t3);
    auto t4_concave = triangulate_concave(t4);
    auto t5_concave = triangulate_concave(t5);
    auto t6_concave = triangulate_concave(t6);
    auto t1_convex = triangulate_convex(t1);
    auto t2_convex = triangulate_convex(t2);
    auto t3_convex = triangulate_convex(t3);
    auto t5_convex = triangulate_convex(t5);
    test_triangulation(t1, t1_concave, "Triangle with concave");
    test_triangulation(t2, t2_concave, "Square with concave");
    test_triangulation(t3, t3_concave, "Reverse square with concave");
    test_triangulation(t4, t4_concave, "Concave with concave");
    test_triangulation(t5, t5_concave, "Collinear with concave", true, true);
    test_triangulation(t6, t6_concave, "Complicated with concave", true);
    test_triangulation(t1, t1_convex, "Triangle with convex");
    test_triangulation(t2, t2_convex, "Square with convex");
    test_triangulation(t3, t3_convex, "Reverse square with convex");
    test_triangulation(t5, t5_convex, "Collinear with convex", true, true);
    REQUIRE(triangulate(t1) == t1_convex);
    REQUIRE(triangulate(t2) == t2_convex);
    REQUIRE(triangulate(t3) == t3_convex);
    REQUIRE(triangulate(t4) == t4_concave);
    REQUIRE(triangulate(t5) == t5_convex);
    REQUIRE(triangulate(t6) == t6_concave);
}
