#include "path.hpp"

#include "ganim/gl/gl.hpp"

#include "ganim/math.hpp"
#include "ganim/ga/exp.hpp"

using namespace ganim;
using namespace pga2;

namespace {
    std::pair<std::vector<Shape::Vertex>, std::vector<unsigned>>
    get_new_vertices(
        pga2::Bivec midpoint,
        pga2::Vec join1,
        pga2::Vec join2,
        double thickness,
        int t
    )
    {
        auto d = thickness / 2.0;
        auto vertices = std::vector<Shape::Vertex>();
        auto indices = std::vector<unsigned>();
        vertices.reserve(3);
        indices.reserve(9);
        auto add_vertex = [&](pga2::Bivec vertex) {
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
        if (std::abs(orientation) < 0.5) {
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
            auto l = pga2::Vec();
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
    const std::vector<pga2::Bivec>& points,
    bool closed,
    double thickness
)
{
    recreate(points, closed, thickness);
}

Path::Path(
    const std::vector<vga2::Vec>& points,
    bool closed,
    double thickness
)
{
    recreate(points, closed, thickness);
}

Path::Path(
    const std::vector<pga3::Trivec>& points,
    int circle_precision,
    bool closed,
    double thickness
)
{
    recreate(points, circle_precision, closed, thickness);
}

Path::Path(
    const std::vector<vga3::Vec>& points,
    int circle_precision,
    bool closed,
    double thickness
)
{
    recreate(points, circle_precision, closed, thickness);
}

void Path::recreate(
    const std::vector<vga2::Vec>& points,
    bool closed,
    double thickness
)
{
    auto new_points = std::vector<pga2::Bivec>();
    new_points.reserve(points.size());
    for (auto p : points) new_points.push_back(vga2_to_pga2(p));
    recreate(new_points, closed, thickness);
}

void Path::recreate(
    const std::vector<pga2::Bivec>& points,
    bool closed,
    double thickness
)
{
    if (points.size() < 2) {
        throw std::invalid_argument("Path must be given at least two points.");
    }
    auto filtered_points = std::vector<pga2::Bivec>();
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
    auto joins = std::vector<pga2::Vec>();
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
    auto add_vertex = [&](pga2::Bivec vertex) {
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

void Path::recreate(
    const std::vector<vga3::Vec>& points,
    int circle_precision,
    bool closed,
    double thickness
)
{
    recreate(vga3_to_pga3(points), circle_precision, closed, thickness);
}

void Path::recreate(
    const std::vector<pga3::Trivec>& points,
    int circle_precision,
    bool closed,
    double thickness
)
{
    using namespace pga3;
    if (points.size() < 2) {
        throw std::invalid_argument("Path must be given at least two points.");
    }

    auto filtered_points = std::vector<pga3::Trivec>();
    filtered_points.reserve(points.size());
    for (auto p : points) {
        auto coef = p.blade_project<e123>();
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
    auto joins = std::vector<pga3::Bivec>();
    joins.reserve(s - 1);
    for (int i = 0; i < s - 1; ++i) {
        auto join = filtered_points[i] & filtered_points[i+1];
        joins.push_back(join.normalized());
    }

    auto d = thickness / 2;
    auto starting_plane = filtered_points[0] | joins[0];
    auto line_plane = (e3 | joins[0]) | joins[0];
    if (line_plane.norm2() < 1e-10) {
        line_plane = (e1 | joins[0]) | joins[0];
    }
    line_plane = line_plane.normalized();
    auto shifted_plane = line_plane + d*e0;
    auto shifted_line = (joins[0] | shifted_plane) ^ shifted_plane;
    auto circle_points = std::vector<pga3::Trivec>();
    circle_points.resize(circle_precision);
    circle_points[0] = -shifted_line ^ starting_plane;
    for (int i = 1; i < circle_precision; ++i) {
        auto r = ga_exp(joins[0] * τ * i / circle_precision);
        circle_points[i] = (~r * circle_points[0] * r).grade_project<3>();
    }
    auto apply_to_circle_points = [&](const auto& rotor) {
        for (auto& p : circle_points) {
            p = (~rotor * p * rotor).template grade_project<3>();
        }
    };

    auto final_points = std::vector<pga3::Trivec>();
    final_points.reserve(circle_precision * filtered_points.size());
    auto add_circle_points = [&](const Even& rotor, const Trivec& midpoint) {
        for (auto p : circle_points) {
            p -= midpoint;
            // The rotor is basically exp(θ/4)
            // cos(θ/4)
            auto c = rotor.blade_project<e>();
            // sin(θ/4)
            auto s = rotor.grade_project<2>().norm();
            // cos(θ/2) (from double angle formula)
            auto c2 = c*c - s*s;
            p /= c2;
            p += midpoint;
            final_points.push_back(p);
        }
    };
    auto r = Even(1);
    if (closed) {
        auto plane1 = (joins.back() | filtered_points[0]).normalized();
        auto plane2 = (joins.front() | filtered_points[0]).normalized();
        auto b = ga_log(plane1 * plane2);
        r = ga_exp(b/4);
        apply_to_circle_points(~r);
    }
    add_circle_points(r, filtered_points[0]);
    if (closed) {
        apply_to_circle_points(r);
    }

    for (int i = 1; i < ssize(filtered_points); ++i) {
        auto r = Even(-filtered_points[i-1]
            * (filtered_points[i-1] + filtered_points[i]) / 2);
        apply_to_circle_points(r);

        if (i != ssize(filtered_points) - 1 or closed) {
            auto plane1 = (joins[i-1] | filtered_points[i]).normalized();
            auto plane2
                = (joins[i % joins.size()] | filtered_points[i]).normalized();
            auto b = ga_log(plane1 * plane2);
            r = ga_exp(b/4);
            apply_to_circle_points(r);
        }
        add_circle_points(r, filtered_points[i]);
        if (i != ssize(filtered_points) - 1 or closed) {
            apply_to_circle_points(r);
        }
        for (auto& p : circle_points) {
            p /= p.blade_project<e123>();
        }
    }

    auto final_vertices = std::vector<Shape::Vertex>();
    auto final_indices = std::vector<unsigned>();
    final_vertices.reserve(final_points.size());
    final_indices.reserve(joins.size() * circle_precision * 6);
    auto t = 0;
    for (auto& p : final_points) {
        auto& v = final_vertices.emplace_back();
        auto pd = p.undual();
        v.x = pd.blade_project<e1>();
        v.y = pd.blade_project<e2>();
        v.z = pd.blade_project<e3>();
        v.t = t++ / circle_precision;
    }
    for (int i = 0; i < ssize(joins); ++i) {
        for (int j = 0; j < circle_precision; ++j) {
            auto i1 = i*circle_precision + j;
            auto i2 = (i+1)*circle_precision + j;
            auto i3 = (i+1)*circle_precision + ((j+1) % circle_precision);
            auto i4 = i*circle_precision + ((j+1) % circle_precision);
            final_indices.push_back(i1);
            final_indices.push_back(i3);
            final_indices.push_back(i2);
            final_indices.push_back(i1);
            final_indices.push_back(i4);
            final_indices.push_back(i3);
        }
    }
    set_vertices(std::move(final_vertices), std::move(final_indices));
}

void Path::set_dash(double on_time, double off_time)
{
    M_dash_on_time = on_time;
    M_dash_off_time = off_time;
}

ShaderFeature Path::get_shader_flags()
{
    auto flags = Shape::get_shader_flags();
    if (M_dash_on_time != 0 and M_dash_off_time != 0) {
        flags |= ShaderFeature::Dash;
    }
    return flags;
}

void Path::set_subclass_uniforms(gl::Shader& shader)
{
    if (M_dash_on_time != 0 and M_dash_off_time != 0) {
        glUniform1f(shader.get_uniform("dash_on_time"), M_dash_on_time);
        glUniform1f(shader.get_uniform("dash_off_time"), M_dash_off_time);
    }
}

ObjectPtr<Path> Path::polymorphic_copy() const
{
    return ObjectPtr<Path>::from_new(polymorphic_copy_impl());
}

Path* Path::polymorphic_copy_impl() const
{
    return new Path(*this);
}
