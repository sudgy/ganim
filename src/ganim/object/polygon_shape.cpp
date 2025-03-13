#include "polygon_shape.hpp"

#include "ganim/util/triangulation.hpp"

using namespace ganim;

namespace {
    std::vector<Shape::Vertex> get_shape_vertices(const auto& vertices)
    {
        auto result = std::vector<Shape::Vertex>();
        result.resize(vertices.size());
        for (int i = 0; i < ssize(vertices); ++i) {
            auto p = vga2::Vec([&]{
                if constexpr (std::is_same_v<
                        decltype(vertices[0]),
                        const vga2::Vec&
                >) {
                    return vertices[i];
                }
                else return pga2_to_vga2(vertices[i]);
            }());
            result[i].x = p.blade_project<vga2::e1>();
            result[i].y = p.blade_project<vga2::e2>();
            result[i].t = i;
        }
        return result;
    }
    std::vector<unsigned> get_shape_indices(const auto& vertices)
    {
        return triangulate(vertices);
    }
}

PolygonShape::PolygonShape(const std::vector<vga2::Vec>& vertices)
:   Shape(get_shape_vertices(vertices), get_shape_indices(vertices)) {}

PolygonShape::PolygonShape(const std::vector<pga2::Bivec>& vertices)
:   Shape(get_shape_vertices(vertices), get_shape_indices(vertices)) {}

ObjectPtr<PolygonShape> PolygonShape::polymorphic_copy() const
{
    return ObjectPtr<PolygonShape>::from_new(polymorphic_copy_impl());
}

PolygonShape* PolygonShape::polymorphic_copy_impl() const
{
    return new PolygonShape(*this);
}
