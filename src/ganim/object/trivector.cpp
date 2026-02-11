#include "trivector.hpp"

#include "ganim/ga/exp.hpp"

using namespace ganim;

namespace {
    ObjectPtr<TypedGroup<ArrowPath>> construct_swirlies(
        std::vector<pga3::Trivec> vertices,
        const std::vector<std::vector<int>>& faces,
        bool clockwise
    )
    {
        using namespace pga3;
        for (auto& p : vertices) {
            p /= p.blade_project<e123>();
        }
        auto result = ObjectPtr<TypedGroup<ArrowPath>>();
        for (auto& face : faces) {
            auto midpoint = Trivec();
            for (auto i : face) {
                midpoint += vertices[i];
            }
            midpoint /= face.size();
            double min_distance = INFINITY;
            for (int i = 0; i < ssize(face); ++i) {
                auto j = (i + 1) % ssize(face);
                auto edge = vertices[face[i]] & vertices[face[j]];
                auto closest = (edge | midpoint) ^ edge;
                closest /= closest.blade_project<e123>();
                auto this_distance = (midpoint - closest).undual().norm2();
                min_distance = std::min(min_distance, this_distance);
            }
            min_distance = std::sqrt(min_distance);

            const auto radius = min_distance * 0.5;
            auto points = std::vector<vga2::Vec>();
            double sign = clockwise ? -1 : 1;
            for (int i = 0; i < 120; ++i) {
                points.push_back(
                    radius*vga2::e1*ga_exp(i/128.0*τ*sign*vga2::e12));
            }
            auto path = make_arrow_path(points);

            auto swirly_plane
                = vertices[face[0]] & vertices[face[1]] & vertices[face[2]];
            swirly_plane /= swirly_plane.norm();
            // Make the swirly slightly outside the polyhedron
            swirly_plane -= e0*min_distance*1e-3;
            auto r = Even();
            auto mid = e3 + swirly_plane;
            if (mid.norm() < 1e-5) {
                r = ga_exp(e13*τ/4);
            }
            else {
                r = e3 * mid;
            }
            auto new_origin = ~r*e123*r;
            new_origin /= new_origin.blade_project<e123>();
            auto new_midpoint = (swirly_plane | midpoint) ^ swirly_plane;
            new_midpoint /= new_midpoint.blade_project<e123>();
            r *= -new_origin * (new_origin + new_midpoint);
            r /= r.norm();
            path->apply_rotor(r);

            result->add(std::move(path));
        }
        return result;
    }
}

Trivector::Trivector(
    const std::vector<vga3::Vec>& vertices,
    const std::vector<std::vector<int>>& faces,
    bool clockwise
)
{
    auto inside = make_polyhedron(vertices, faces);
    auto outside = construct_swirlies(to_pga3(vertices), faces, clockwise);
    set(std::move(inside), std::move(outside));
}

Trivector::Trivector(
    const std::vector<pga3::Trivec>& vertices,
    const std::vector<std::vector<int>>& faces,
    bool clockwise
)
{
    auto inside = make_polyhedron(vertices, faces);
    auto outside = construct_swirlies(vertices, faces, clockwise);
    set(std::move(inside), std::move(outside));
}

Trivector::Trivector(double width, double height, double depth, bool clockwise)
{
    using namespace pga3;
    auto vertices = std::vector<Trivec>();
    vertices.reserve(8);
    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 2; ++y) {
            for (int z = 0; z < 2; ++z) {
                vertices.push_back(
                    x*width*e1.dual() + y*height*e2.dual() + z*depth*e3.dual()
                    + e123
                );
            }
        }
    }
    auto faces = std::vector<std::vector<int>>{
        {1, 5, 7, 3},
        {0, 4, 5, 1},
        {4, 6, 7, 5},
        {6, 2, 3, 7},
        {2, 0, 1, 3},
        {0, 2, 6, 4}
    };
    auto inside = make_polyhedron(vertices, faces);
    auto outside = construct_swirlies(vertices, faces, clockwise);
    set(std::move(inside), std::move(outside));
}

Trivector::Trivector(vga3::Trivec trivector)
:   Trivector(
        std::cbrt(trivector.norm()),
        std::cbrt(trivector.norm()),
        std::cbrt(trivector.norm()),
        trivector.blade_project<vga3::e123>() < 0
    ) {}

ObjectPtr<Trivector> Trivector::copy() const
{
    return ObjectPtr<Trivector>::from_new(copy_impl());
}

Trivector* Trivector::copy_impl() const
{
    return new Trivector(*this);
}

Trivector& Trivector::set_color(Color color)
{
    Object::set_color(color);
    if (propagate()) {
        get_inside()->set_color(color);
    }
    return *this;
}
