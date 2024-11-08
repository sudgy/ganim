#ifndef GANIM_UTIL_BOUNDING_BOX_HPP
#define GANIM_UTIL_BOUNDING_BOX_HPP

#include "ganim/ga/conversions.hpp"

namespace ganim {

class Box {
    public:
        Box();
        Box(const pointlike auto& p1, const pointlike auto& p2)
            : Box(pointlike_to_pga3(p1), pointlike_to_pga3(p2)) {}
        Box(const pga3::Trivector& p1, const pga3::Trivector& p2);

        pga3::Vector get_left_face() const;
        pga3::Vector get_right_face() const;
        pga3::Vector get_up_face() const;
        pga3::Vector get_down_face() const;
        pga3::Vector get_out_face() const;
        pga3::Vector get_in_face() const;
        pga3::Bivector get_upper_left_edge() const;
        pga3::Bivector get_upper_right_edge() const;
        pga3::Bivector get_lower_left_edge() const;
        pga3::Bivector get_lower_right_edge() const;
        pga3::Bivector get_outer_left_edge() const;
        pga3::Bivector get_outer_right_edge() const;
        pga3::Bivector get_outer_up_edge() const;
        pga3::Bivector get_outer_down_edge() const;
        pga3::Bivector get_inner_left_edge() const;
        pga3::Bivector get_inner_right_edge() const;
        pga3::Bivector get_inner_up_edge() const;
        pga3::Bivector get_inner_down_edge() const;
        pga3::Trivector get_outer_upper_left_vertex() const;
        pga3::Trivector get_outer_upper_right_vertex() const;
        pga3::Trivector get_outer_lower_left_vertex() const;
        pga3::Trivector get_outer_lower_right_vertex() const;
        pga3::Trivector get_inner_upper_left_vertex() const;
        pga3::Trivector get_inner_upper_right_vertex() const;
        pga3::Trivector get_inner_lower_left_vertex() const;
        pga3::Trivector get_inner_lower_right_vertex() const;
        pga3::Trivector get_center_3d() const;
        pga3::Trivector get_left_3d() const;
        pga3::Trivector get_right_3d() const;
        pga3::Trivector get_up_3d() const;
        pga3::Trivector get_down_3d() const;
        pga3::Trivector get_out_3d() const;
        pga3::Trivector get_in_3d() const;

        pga2::Vector get_left_edge() const;
        pga2::Vector get_right_edge() const;
        pga2::Vector get_up_edge() const;
        pga2::Vector get_down_edge() const;
        pga2::Bivector get_center() const;
        pga2::Bivector get_left() const;
        pga2::Bivector get_right() const;
        pga2::Bivector get_up() const;
        pga2::Bivector get_down() const;
        pga2::Bivector get_upper_left() const;
        pga2::Bivector get_upper_right() const;
        pga2::Bivector get_lower_left() const;
        pga2::Bivector get_lower_right() const;

        pga2::Bivector get_outside_point(const pga2::Bivector& point) const;
        pga2::Bivector get_outside_point(const pointlike auto& point)const
            {return get_outside_point(pga3_to_pga2(pointlike_to_pga3(point)));}
        pga3::Trivector get_outside_point_3d(const pga3::Trivector& point)const;
        pga3::Trivector get_outside_point_3d(const pointlike auto& point)const
            {return get_outside_point_3d(pointlike_to_pga3(point));}

    private:
        pga3::Trivector p1 = pga3::e123;
        pga3::Trivector p2 = pga3::e123;
};

Box merge_boxes(const Box& box1, const Box& box2);
Box transform_box(const Box& box, const pga3::Even& rotor, double scale = 1);

}

#endif
