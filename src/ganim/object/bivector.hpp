#ifndef GANIM_OBJECT_BIVECTOR_HPP
#define GANIM_OBJECT_BIVECTOR_HPP

#include "ganim/object/bases/group.hpp"
#include "ganim/object/polygon_shape.hpp"
#include "ganim/object/arrow_path.hpp"

namespace ganim {
    class Vector;
    struct BivectorArgs {
        double max_tip_to_length_ratio = 0.5;
        double tip_size = 0.3;
        double thickness = 0.06;
    };
    class Bivector : public Group {
        public:
            Bivector(
                const std::vector<pga2::Bivec>& points,
                BivectorArgs args = {}
            );
            Bivector(
                const std::vector<vga2::Vec>& points,
                BivectorArgs args = {}
            );
            Bivector(
                const Vector& v1,
                const Vector& v2,
                BivectorArgs args = {}
            );
            Bivector(
                vga2::Vec v1,
                vga2::Vec v2,
                BivectorArgs args = {}
            );
            Bivector(
                vga3::Vec v1,
                vga3::Vec v2,
                BivectorArgs args = {}
            );
            Bivector(
                vga2::Bivec b,
                BivectorArgs args = {}
            );
            Bivector(
                vga3::Bivec b,
                BivectorArgs args = {}
            );
            ObjectPtr<Group> get_outside() {return M_outside;}
            ObjectPtr<PolygonShape> get_inside() {return M_inside;}

        private:
            void common_construct(
                const std::vector<pga2::Bivec>& pga_points,
                const std::vector<vga2::Vec>& vga_points,
                BivectorArgs args
            );
            ObjectPtr<Group> M_outside = nullptr;
            ObjectPtr<PolygonShape> M_inside = nullptr;
            std::vector<ObjectPtr<ArrowPath>> M_outside_paths;
    };

    inline ObjectPtr<Bivector> make_bivector(
        const std::vector<pga2::Bivec>& points,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(points, args);
    }
    inline ObjectPtr<Bivector> make_bivector(
        const std::vector<vga2::Vec>& points,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(points, args);
    }
    inline ObjectPtr<Bivector> make_bivector(
        const Vector& v1,
        const Vector& v2,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(v1, v2, args);
    }
    inline ObjectPtr<Bivector> make_bivector(
        vga2::Vec v1,
        vga2::Vec v2,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(v1, v2, args);
    }
    inline ObjectPtr<Bivector> make_bivector(
        vga3::Vec v1,
        vga3::Vec v2,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(v1, v2, args);
    }
    inline ObjectPtr<Bivector> make_bivector(
        vga2::Bivec b,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(b, args);
    }
    inline ObjectPtr<Bivector> make_bivector(
        vga3::Bivec b,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(b, args);
    }
}

#endif
