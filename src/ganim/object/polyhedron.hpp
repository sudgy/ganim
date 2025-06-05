#ifndef GANIM_OBJECT_POLYHEDRON_HPP
#define GANIM_OBJECT_POLYHEDRON_HPP

#include "ganim/object/bases/static_group.hpp"

namespace ganim {
    class Polyhedron : public StaticGroup<Group, Group> {
        public:
            explicit Polyhedron(
                const std::vector<vga3::Vec>& vertices,
                const std::vector<std::vector<int>>& faces
            );
            explicit Polyhedron(
                const std::vector<pga3::Trivec>& vertices,
                const std::vector<std::vector<int>>& faces
            );

            ObjectPtr<Polyhedron> polymorphic_copy() const;

            virtual Polyhedron& set_color(Color color) override;

            ObjectPtr<Group> get_faces() const {return get<0>();}
            ObjectPtr<Group> get_edges() const {return get<1>();}

        private:
            virtual Polyhedron* polymorphic_copy_impl() const override;
    };
    inline ObjectPtr<Polyhedron> make_polyhedron(
        const std::vector<vga3::Vec>& vertices,
        const std::vector<std::vector<int>>& faces
    )
    {
        return ObjectPtr<Polyhedron>(vertices, faces);
    }
    inline ObjectPtr<Polyhedron> make_polyhedron(
        const std::vector<pga3::Trivec>& vertices,
        const std::vector<std::vector<int>>& faces
    )
    {
        return ObjectPtr<Polyhedron>(vertices, faces);
    }
}

#endif
