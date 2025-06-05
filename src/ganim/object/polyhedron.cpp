#include "polyhedron.hpp"

#include <unordered_set>

#include "ganim/object/polygon_shape.hpp"
#include "ganim/object/path.hpp"

using namespace ganim;

namespace {
    std::pair<ObjectPtr<Group>, ObjectPtr<Group>>
    get_parts(
        const auto& vertices,
        const std::vector<std::vector<int>>& face_indices
    )
    {
        auto faces = make_group();
        auto edges = make_group();
        auto accounted_edges = std::unordered_set<std::uint64_t>();

        for (auto& face : face_indices) {
            auto this_vertices = std::remove_cvref_t<decltype(vertices)>();
            this_vertices.reserve(face.size());
            for (auto index : face) {
                this_vertices.push_back(vertices.at(index));
            }
            auto new_face = make_polygon_shape(this_vertices);
            new_face->do_shading(true);
            faces->add(new_face);

            for (int i = 0; i < ssize(face); ++i) {
                int j = (i + 1) % ssize(face);
                auto m = std::min(face[i], face[j]);
                auto n = std::max(face[i], face[j]);
                auto key = (std::uint64_t(m) << 32U) + std::uint64_t(n);
                if (accounted_edges.emplace(key).second) {
                    auto p1 = vertices[m];
                    auto p2 = vertices[n];
                    edges->add(make_path({p1, p2}, 8));
                }
            }
        }
        faces->set_color("FFFFFF9F");

        return {faces, edges};
    }
}

Polyhedron::Polyhedron(
    const std::vector<vga3::Vec>& vertices,
    const std::vector<std::vector<int>>& faces
)
{
    auto parts = get_parts(vertices, faces);
    set(parts.first, parts.second);
}

Polyhedron::Polyhedron(
    const std::vector<pga3::Trivec>& vertices,
    const std::vector<std::vector<int>>& faces
)
{
    auto parts = get_parts(vertices, faces);
    set(parts.first, parts.second);
}

Polyhedron& Polyhedron::set_color(Color color)
{
    Object::set_color(color);
    if (propagate()) {
        get_edges()->set_color(color);
        color.a *= 0.625;
        get_faces()->set_color(color);
    }
    return *this;
}

ObjectPtr<Polyhedron> Polyhedron::polymorphic_copy() const
{
    return ObjectPtr<Polyhedron>::from_new(polymorphic_copy_impl());
}

Polyhedron* Polyhedron::polymorphic_copy_impl() const
{
    auto result = std::make_unique<Polyhedron>(*this);
    result->set(
        get_faces()->polymorphic_copy(),
        get_edges()->polymorphic_copy()
    );
    return result.release();
}
