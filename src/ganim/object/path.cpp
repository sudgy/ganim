#include "path.hpp"

using namespace ganim;
using namespace pga2;

namespace {
    std::pair<std::vector<Shape::Vertex>, std::vector<unsigned>>
    get_new_vertices(
        pga2::Bivector midpoint,
        pga2::Vector join1,
        pga2::Vector join2,
        double thickness,
        int t
    )
    {
        auto d = thickness / 2.0;
        auto vertices = std::vector<Shape::Vertex>();
        auto indices = std::vector<unsigned>();
        vertices.reserve(3);
        indices.reserve(9);
        auto add_vertex = [&](pga2::Bivector vertex) {
            auto p = vertex.undual();
            p /= p.blade_project<e0>();
            auto x = float(p.blade_project<e1>());
            auto y = float(p.blade_project<e2>());
            vertices.push_back({x, y, 0, float(t)});
        };
        auto add_triangle = [&](unsigned i1, unsigned i2, unsigned i3) {
            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        };
        auto orientation = (join1 ^ join2).blade_project<e12>();
        auto direction = (join1 | join2).blade_project<e>();
        // The three points are roughly collinear
        if (std::abs(orientation) < 1e-1) {
            auto b = midpoint | join2;
            add_vertex((join2 - d*e0) ^ b);
            add_vertex((join2 + d*e0) ^ b);
            // The path at this point is just a straight line
            if (direction > 0) {
                add_triangle(0, 2, 1);
                add_triangle(1, 2, 3);
            }
            // The path turned almost completely 180 for some reason.  The
            // vertices used need to be flipped.
            else {
                add_triangle(0, 3, 1);
                add_triangle(1, 3, 2);
            }
        }
        // The three points are not roughly collinear
        else {
            auto b = midpoint;
            auto m1 = join1 - d*e0;
            auto m2 = join1 + d*e0;
            auto n1 = join2 - d*e0;
            auto n2 = join2 + d*e0;
            // The line passing through b, perpendicular to the bisector of
            // the two line segments ending on this point
            auto l = pga2::Vector();
            // The outside points are generally on opposite ends of the middle
            // point
            if (direction > 0) {
                auto a = b - join1*e0;
                auto c = b + join2*e0;
                l = ((a & c) | b) | -b;
            }
            // The outside points are generally on the same side of the middle
            // point
            else {
                l = b | (join1 - join2);
                if (orientation > 0) l *= -1;
            }
            // Turn left
            if (orientation > 0) {
                add_vertex(m2 ^ (l + d*e0));
                add_vertex(m1 ^ n1);
                add_vertex(n2 ^ (l + d*e0));
                add_triangle(0, 3, 1);
                add_triangle(1, 3, 2);
                add_triangle(2, 3, 4);
            }
            // Turn right
            else {
                add_vertex(m1 ^ (l - d*e0));
                add_vertex(n1 ^ (l - d*e0));
                add_vertex(m2 ^ n2);
                add_triangle(0, 2, 1);
                add_triangle(1, 2, 4);
                add_triangle(2, 3, 4);
            }
        }
        return {vertices, indices};
    }
}

Path::Path(
    const std::vector<pga2::Bivector>& points,
    bool closed,
    double thickness
)
{
    recreate(points, closed, thickness);
}

Path::Path(
    const std::vector<vga2::Vector>& points,
    bool closed,
    double thickness
)
{
    recreate(points, closed, thickness);
}

void Path::recreate(
    const std::vector<vga2::Vector>& points,
    bool closed,
    double thickness
)
{
    auto new_points = std::vector<pga2::Bivector>();
    new_points.reserve(points.size());
    for (auto p : points) new_points.push_back(vga2_to_pga2(p));
    recreate(new_points, closed, thickness);
}

void Path::recreate(
    const std::vector<pga2::Bivector>& points,
    bool closed,
    double thickness
)
{
    if (points.size() < 2) {
        throw std::invalid_argument("Path must be given at least two points.");
    }
    auto filtered_points = std::vector<pga2::Bivector>();
    filtered_points.reserve(points.size());
    for (auto p : points) {
        auto coef = p.blade_project<e12>();
        if (coef == 0) {
            throw std::invalid_argument(
                    "Path doesn't support points at infinity.");
        }
        auto normalized = p / coef;
        // Maybe somebody is making a path on a function that starts out
        // stationary or something.  I don't know, but the following algorithm
        // breaks on duplicate points, and I want to support them, so just
        // remove any duplicate points.
        if (filtered_points.empty() or filtered_points.back() != normalized) {
            filtered_points.push_back(normalized);
        }
    }
    if (closed) filtered_points.push_back(filtered_points.front());
    auto s = ssize(filtered_points);
    // Get all lines joining the input points together.  Note that like fence
    // length vs. fence posts, there is one less join line than points.  These
    // need to be normalized for future calculations.
    auto joins = std::vector<pga2::Vector>();
    joins.reserve(s - 1);
    for (int i = 0; i < s - 1; ++i) {
        auto join = filtered_points[i] & filtered_points[i+1];
        joins.push_back(join.normalized());
    }

    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<unsigned>();
    vertices.reserve(s*3);
    indices.reserve(s*9);
    auto t = 0;
    auto n = 0; // Vertex index
    auto add_vertex = [&](pga2::Bivector vertex) {
        auto p = vertex.undual();
        p /= p.blade_project<e0>();
        auto x = float(p.blade_project<e1>());
        auto y = float(p.blade_project<e2>());
        vertices.push_back({x, y, 0, float(t)});
    };
    auto add_triangle = [&](unsigned i1, unsigned i2, unsigned i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    };
    auto d = thickness / 2.0;
    // Starting point
    {
        if (closed) {
            auto [new_vertices, new_indices] = get_new_vertices(
                filtered_points[0],
                joins[s-2],
                joins[0],
                thickness,
                t
            );
            // Only add the last two because the end will take care of the
            // optional extra triangle
            vertices.push_back(new_vertices.rbegin()[1]);
            vertices.push_back(new_vertices.rbegin()[0]);
        }
        else {
            auto b = filtered_points[0] | joins[0];
            add_vertex((joins[0] - d*e0) ^ b);
            add_vertex((joins[0] + d*e0) ^ b);
        }
    }
    ++t;
    // Intermediate points
    for (int i = 1; i < s - 1; ++i) {
        auto [new_vertices, new_indices] = get_new_vertices(
            filtered_points[i],
            joins[i-1],
            joins[i],
            thickness,
            t
        );
        for (const auto& vertex : new_vertices) {
            vertices.push_back(vertex);
        }
        for (auto index : new_indices) {
            indices.push_back(n + index);
        }
        n += new_indices.size() / 3;
        ++t;
    }
    // Ending point
    {
        if (closed) {
            auto [new_vertices, new_indices] = get_new_vertices(
                filtered_points[0],
                joins[s-2],
                joins[0],
                thickness,
                t
            );
            for (const auto& vertex : new_vertices) {
                vertices.push_back(vertex);
            }
            for (auto index : new_indices) {
                indices.push_back(n + index);
            }
        }
        else {
            auto b = filtered_points[s-1] | joins[s-2];
            add_vertex((joins[s-2] - d*e0) ^ b);
            add_vertex((joins[s-2] + d*e0) ^ b);
            add_triangle(n  , n+2, n+1);
            add_triangle(n+1, n+2, n+3);
        }
    }
    set_vertices(std::move(vertices), std::move(indices));
}
