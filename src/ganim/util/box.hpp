#ifndef GANIM_UTIL_BOUNDING_BOX_HPP
#define GANIM_UTIL_BOUNDING_BOX_HPP

#include "ganim/ga/conversions.hpp"

namespace ganim {

class Box {
    public:
        Box();
        Box(const pointlike auto& p1, const pointlike auto& p2)
            : Box(pointlike_to_pga3(p1), pointlike_to_pga3(p2)) {}
        Box(const pga3::Trivec& p1, const pga3::Trivec& p2);

        pga3::Vec get_left_face() const;
        pga3::Vec get_right_face() const;
        pga3::Vec get_up_face() const;
        pga3::Vec get_down_face() const;
        pga3::Vec get_out_face() const;
        pga3::Vec get_in_face() const;
        pga3::Bivec get_upper_left_edge() const;
        pga3::Bivec get_upper_right_edge() const;
        pga3::Bivec get_lower_left_edge() const;
        pga3::Bivec get_lower_right_edge() const;
        pga3::Bivec get_outer_left_edge() const;
        pga3::Bivec get_outer_right_edge() const;
        pga3::Bivec get_outer_up_edge() const;
        pga3::Bivec get_outer_down_edge() const;
        pga3::Bivec get_inner_left_edge() const;
        pga3::Bivec get_inner_right_edge() const;
        pga3::Bivec get_inner_up_edge() const;
        pga3::Bivec get_inner_down_edge() const;
        pga3::Trivec get_outer_upper_left_vertex() const;
        pga3::Trivec get_outer_upper_right_vertex() const;
        pga3::Trivec get_outer_lower_left_vertex() const;
        pga3::Trivec get_outer_lower_right_vertex() const;
        pga3::Trivec get_inner_upper_left_vertex() const;
        pga3::Trivec get_inner_upper_right_vertex() const;
        pga3::Trivec get_inner_lower_left_vertex() const;
        pga3::Trivec get_inner_lower_right_vertex() const;
        pga3::Trivec get_center_3d() const;
        pga3::Trivec get_left_3d() const;
        pga3::Trivec get_right_3d() const;
        pga3::Trivec get_up_3d() const;
        pga3::Trivec get_down_3d() const;
        pga3::Trivec get_out_3d() const;
        pga3::Trivec get_in_3d() const;

        pga2::Vec get_left_edge() const;
        pga2::Vec get_right_edge() const;
        pga2::Vec get_up_edge() const;
        pga2::Vec get_down_edge() const;
        pga2::Bivec get_center() const;
        pga2::Bivec get_left() const;
        pga2::Bivec get_right() const;
        pga2::Bivec get_up() const;
        pga2::Bivec get_down() const;
        pga2::Bivec get_upper_left() const;
        pga2::Bivec get_upper_right() const;
        pga2::Bivec get_lower_left() const;
        pga2::Bivec get_lower_right() const;

        pga2::Bivec get_outside_point(const pga2::Bivec& point) const;
        pga2::Bivec get_outside_point(const pointlike auto& point)const
            {return get_outside_point(pga3_to_pga2(pointlike_to_pga3(point)));}
        pga3::Trivec get_outside_point_3d(const pga3::Trivec& point)const;
        pga3::Trivec get_outside_point_3d(const pointlike auto& point)const
            {return get_outside_point_3d(pointlike_to_pga3(point));}

        double get_width() const;
        double get_height() const;
        double get_thickness() const;

    private:
        pga3::Trivec p1 = pga3::e123;
        pga3::Trivec p2 = pga3::e123;
};

Box merge_boxes(const Box& box1, const Box& box2);
Box transform_box(const Box& box, const pga3::Even& rotor, double scale = 1);

}

#endif
