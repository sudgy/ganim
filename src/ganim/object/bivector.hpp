#ifndef GANIM_OBJECT_BIVECTOR_HPP
#define GANIM_OBJECT_BIVECTOR_HPP

#include "ganim/object/bases/group.hpp"
#include "ganim/object/polygon_shape.hpp"
#include "ganim/object/arrow_path.hpp"

namespace ganim {
    class VectorObject;
    struct BivectorArgs {
        double max_tip_to_length_ratio = 0.5;
        double tip_size = 0.3;
        double thickness = 0.06;
    };
    class BivectorObject : public Group {
        public:
            BivectorObject(
                const std::vector<pga2::Bivector>& points,
                BivectorArgs args = {}
            );
            BivectorObject(
                const std::vector<vga2::Vector>& points,
                BivectorArgs args = {}
            );
            BivectorObject(
                const VectorObject& v1,
                const VectorObject& v2,
                BivectorArgs args = {}
            );
            BivectorObject(
                vga2::Vector v1,
                vga2::Vector v2,
                BivectorArgs args = {}
            );
            BivectorObject(
                vga3::Vector v1,
                vga3::Vector v2,
                BivectorArgs args = {}
            );
            BivectorObject(
                vga2::Bivector b,
                BivectorArgs args = {}
            );
            BivectorObject(
                vga3::Bivector b,
                BivectorArgs args = {}
            );
            Group& get_outside() {return *M_outside;}
            PolygonShape& get_inside() {return *M_inside;}

        private:
            void common_construct(
                const std::vector<pga2::Bivector>& pga_points,
                const std::vector<vga2::Vector>& vga_points,
                BivectorArgs args
            );
            std::unique_ptr<Group> M_outside;
            std::unique_ptr<PolygonShape> M_inside;
            std::vector<ArrowPath> M_outside_paths;
    };
}

#endif
