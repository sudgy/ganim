#ifndef GANIM_OBJECT_TRIVECTOR_HPP
#define GANIM_OBJECT_TRIVECTOR_HPP

#include "polyhedron.hpp"
#include "arrow_path.hpp"

namespace ganim {
    class Trivector :
            public StaticGroup<Polyhedron, TypedGroup<ArrowPath>> {
        public:
            Trivector(
                const std::vector<vga3::Vec>& vertices,
                const std::vector<std::vector<int>>& faces,
                bool clockwise = false
            );
            Trivector(
                const std::vector<pga3::Trivec>& vertices,
                const std::vector<std::vector<int>>& faces,
                bool clockwise = false
            );
            Trivector(
                double width,
                double height,
                double depth,
                bool clockwise = false
            );
            Trivector(vga3::Trivec trivector);

            ObjectPtr<Trivector> polymorphic_copy() const;

            virtual Trivector& set_color(Color color) override;

            ObjectPtr<Polyhedron> get_inside() const
                {return get<0>();}
            ObjectPtr<TypedGroup<ArrowPath>> get_outside() const
                {return get<1>();}

        private:
            virtual Trivector* polymorphic_copy_impl() const override;
    };
    inline ObjectPtr<Trivector> make_trivector(
        const std::vector<vga3::Vec>& vertices,
        const std::vector<std::vector<int>>& faces,
        bool clockwise = false
    )
    {
        return ObjectPtr<Trivector>(vertices, faces, clockwise);
    }
    inline ObjectPtr<Trivector> make_trivector(
        const std::vector<pga3::Trivec>& vertices,
        const std::vector<std::vector<int>>& faces,
        bool clockwise = false
    )
    {
        return ObjectPtr<Trivector>(vertices, faces, clockwise);
    }
    inline ObjectPtr<Trivector> make_trivector(
        double width,
        double height,
        double depth,
        bool clockwise = false
    )
    {
        return ObjectPtr<Trivector>(width, height, depth, clockwise);
    }
    inline ObjectPtr<Trivector> make_trivector(vga3::Trivec trivector)
    {
        return ObjectPtr<Trivector>(trivector);
    }
}

#endif
