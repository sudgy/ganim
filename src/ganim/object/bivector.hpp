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
            Group& get_outside() {return *M_outside;}
            PolygonShape& get_inside() {return *M_inside;}

        private:
            void common_construct(
                const std::vector<pga2::Bivec>& pga_points,
                const std::vector<vga2::Vec>& vga_points,
                BivectorArgs args
            );
            std::unique_ptr<Group> M_outside;
            std::unique_ptr<PolygonShape> M_inside;
            std::vector<ArrowPath> M_outside_paths;
    };
}

#endif
