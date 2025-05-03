#ifndef GANIM_OBJECT_POLYHEDRON_HPP
#define GANIM_OBJECT_POLYHEDRON_HPP

#include "ganim/object/bases/group.hpp"

namespace ganim {
    class Polyhedron : public Group {
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

            const ObjectPtr<Group>& get_faces() const {return M_faces;}
            const ObjectPtr<Group>& get_edges() const {return M_edges;}

        private:
            virtual Polyhedron* polymorphic_copy_impl() const override;

            ObjectPtr<Group> M_faces;
            ObjectPtr<Group> M_edges;
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
