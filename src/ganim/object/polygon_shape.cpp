#include "polygon_shape.hpp"

#include "ganim/util/triangulation.hpp"

using namespace ganim;

namespace {
    std::vector<Shape::Vertex> get_shape_vertices(const auto& vertices)
    {
        using namespace vga3;
        auto result = std::vector<Shape::Vertex>();
        result.resize(vertices.size());
        for (int i = 0; i < ssize(vertices); ++i) {
            auto p = Vec([&]{
                if constexpr (std::is_same_v<
                        decltype(vertices[0]),
                        const vga3::Vec&
                >) {
                    return vertices[i];
                }
                else if constexpr (std::is_same_v<
                        decltype(vertices[0]),
                        const pga3::Trivec&
                >) {
                    return pga3_to_vga3(vertices[i]);
                }
                else if constexpr (std::is_same_v<
                        decltype(vertices[0]),
                        const vga2::Vec&
                >) {
                    return vga2_to_vga3(vertices[i]);
                }
                else return vga2_to_vga3(pga2_to_vga2(vertices[i]));
            }());
            result[i].x = p.blade_project<e1>();
            result[i].y = p.blade_project<e2>();
            result[i].z = p.blade_project<e3>();
            result[i].t = i;
        }
        return result;
    }
    std::vector<unsigned> get_shape_indices(const auto& vertices)
    {
        return triangulate(vertices);
    }
    std::vector<unsigned> get_shape_indices(
        std::vector<vga3::Vec> vertices
    )
    {
        using namespace vga3;
        auto polygon_plane = [&]{
            for (int i = 0; i < ssize(vertices); ++i) {
                for (int j = i + 1; j < ssize(vertices); ++j) {
                    for (int k = j + 1; k < ssize(vertices); ++k) {
                        auto p1 = vertices[i];
                        auto p2 = vertices[j];
                        auto p3 = vertices[k];
                        auto plane = (p2 - p1) ^ (p3 - p2);
                        if (plane.norm2() != 0.0) return plane.normalized();
                    }
                }
            }
            throw std::invalid_argument(
                "Unable to find the plane that a 3D polygon is in.  "
                "This could be caused by passing in fewer than three "
                "points or by all of the points being collinear.");
        }();
        if ((polygon_plane + e12).norm2() > 1e-10) {
            auto rotor = -polygon_plane * (polygon_plane + e12).normalized();
            for (auto& v : vertices) {
                v = (~rotor * v * rotor).grade_project<1>();
            }
        }
        auto vga2_vertices = std::vector<vga2::Vec>();
        vga2_vertices.resize(vertices.size());
        for (int i = 0; i < ssize(vertices); ++i) {
            auto& v = vertices[i];
            vga2_vertices[i] = v.blade_project<e1>() * vga2::e1 +
                               v.blade_project<e2>() * vga2::e2;
        }
        return get_shape_indices(vga2_vertices);
    }
    std::vector<unsigned> get_shape_indices(
        const std::vector<pga3::Trivec>& vertices
    )
    {
        auto vga_vertices = std::vector<vga3::Vec>();
        vga_vertices.resize(vertices.size());
        for (int i = 0; i < ssize(vertices); ++i) {
            vga_vertices[i] = pga3_to_vga3(vertices[i]);
        }
        return get_shape_indices(vga_vertices);
    }
}

PolygonShape::PolygonShape(const std::vector<vga2::Vec>& vertices)
:   Shape(get_shape_vertices(vertices), get_shape_indices(vertices)) {}

PolygonShape::PolygonShape(const std::vector<pga2::Bivec>& vertices)
:   Shape(get_shape_vertices(vertices), get_shape_indices(vertices)) {}

PolygonShape::PolygonShape(const std::vector<vga3::Vec>& vertices)
:   Shape(get_shape_vertices(vertices), get_shape_indices(vertices)) {}

PolygonShape::PolygonShape(const std::vector<pga3::Trivec>& vertices)
:   Shape(get_shape_vertices(vertices), get_shape_indices(vertices)) {}

ObjectPtr<PolygonShape> PolygonShape::polymorphic_copy() const
{
    return ObjectPtr<PolygonShape>::from_new(polymorphic_copy_impl());
}

PolygonShape* PolygonShape::polymorphic_copy_impl() const
{
    return new PolygonShape(*this);
}
