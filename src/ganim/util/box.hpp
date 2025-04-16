#ifndef GANIM_UTIL_BOUNDING_BOX_HPP
#define GANIM_UTIL_BOUNDING_BOX_HPP

#include "ganim/ga/conversions.hpp"

namespace ganim {

class BoxInterface {
    public:
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

        pga2::Vec get_left_edge() const;
        pga2::Vec get_right_edge() const;
        pga2::Vec get_up_edge() const;
        pga2::Vec get_down_edge() const;

        template <GAFlavor flavor = PGA3>
        auto get_outer_upper_left() const
            {return ga_convert<flavor>(get_outer_upper_left_base());}
        template <GAFlavor flavor = PGA3>
        auto get_outer_upper_right() const
            {return ga_convert<flavor>(get_outer_upper_right_base());}
        template <GAFlavor flavor = PGA3>
        auto get_outer_lower_left() const
            {return ga_convert<flavor>(get_outer_lower_left_base());}
        template <GAFlavor flavor = PGA3>
        auto get_outer_lower_right() const
            {return ga_convert<flavor>(get_outer_lower_right_base());}
        template <GAFlavor flavor = PGA3>
        auto get_inner_upper_left() const
            {return ga_convert<flavor>(get_inner_upper_left_base());}
        template <GAFlavor flavor = PGA3>
        auto get_inner_upper_right() const
            {return ga_convert<flavor>(get_inner_upper_right_base());}
        template <GAFlavor flavor = PGA3>
        auto get_inner_lower_left() const
            {return ga_convert<flavor>(get_inner_lower_left_base());}
        template <GAFlavor flavor = PGA3>
        auto get_inner_lower_right() const
            {return ga_convert<flavor>(get_inner_lower_right_base());}
        template <GAFlavor flavor = PGA2>
        auto get_center() const
            {return ga_convert<flavor>(get_center_base());}
        template <GAFlavor flavor = PGA2>
        auto get_left() const
            {return ga_convert<flavor>(get_left_base());}
        template <GAFlavor flavor = PGA2>
        auto get_right() const
            {return ga_convert<flavor>(get_right_base());}
        template <GAFlavor flavor = PGA2>
        auto get_up() const
            {return ga_convert<flavor>(get_up_base());}
        template <GAFlavor flavor = PGA2>
        auto get_down() const
            {return ga_convert<flavor>(get_down_base());}
        template <GAFlavor flavor = PGA3>
        auto get_out() const
            {return ga_convert<flavor>(get_out_base());}
        template <GAFlavor flavor = PGA3>
        auto get_in() const
            {return ga_convert<flavor>(get_in_base());}
        template <GAFlavor flavor = PGA2>
        auto get_upper_left() const
            {return ga_convert<flavor>(get_upper_left_base());}
        template <GAFlavor flavor = PGA2>
        auto get_upper_right() const
            {return ga_convert<flavor>(get_upper_right_base());}
        template <GAFlavor flavor = PGA2>
        auto get_lower_left() const
            {return ga_convert<flavor>(get_lower_left_base());}
        template <GAFlavor flavor = PGA2>
        auto get_lower_right() const
            {return ga_convert<flavor>(get_lower_right_base());}

        /** @brief Get the point on the boundary of the box in a certain
         * direction, assuming a "2D Box".
         *
         * Both ideal and non-ideal points are supported.  Non-ideal points will
         * be considered to be ideal points from the perspective of the origin.
         */
        template <GAFlavor flavor = PGA2>
        auto get_outside_point(const pointlike auto& point)const
            {return ga_convert<flavor>(get_outside_point_base(
                        pointlike_to_pga3( point)));}

        double get_width() const;
        double get_height() const;
        double get_thickness() const;

    private:
        pga3::Trivec get_outer_upper_left_base() const;
        pga3::Trivec get_outer_upper_right_base() const;
        pga3::Trivec get_outer_lower_left_base() const;
        pga3::Trivec get_outer_lower_right_base() const;
        pga3::Trivec get_inner_upper_left_base() const;
        pga3::Trivec get_inner_upper_right_base() const;
        pga3::Trivec get_inner_lower_left_base() const;
        pga3::Trivec get_inner_lower_right_base() const;
        pga3::Trivec get_center_base() const;
        pga3::Trivec get_left_base() const;
        pga3::Trivec get_right_base() const;
        pga3::Trivec get_up_base() const;
        pga3::Trivec get_down_base() const;
        pga3::Trivec get_out_base() const;
        pga3::Trivec get_in_base() const;
        pga3::Trivec get_upper_left_base() const;
        pga3::Trivec get_upper_right_base() const;
        pga3::Trivec get_lower_left_base() const;
        pga3::Trivec get_lower_right_base() const;
        pga3::Trivec get_outside_point_base(const pga3::Trivec& point)const;

        virtual std::pair<pga3::Trivec, pga3::Trivec> get_box() const=0;
};

/** @brief Represents a box in 3D space.
 *
 * It's constructed with two points, the point with the smallest x, y, and z
 * coordinates, and the point with the largest x, y, and z coordinates.  This
 * class has tons of functions that are hopefully self-explanatory so I'm not
 * going to bother documenting most of it.
 */
class Box : public BoxInterface {
    public:
        /** @brief Creates a box at the origin with no size. */
        Box();
        Box(const pointlike auto& p1, const pointlike auto& p2)
            : Box(pointlike_to_pga3(p1), pointlike_to_pga3(p2)) {}
        Box(const pga3::Trivec& p1, const pga3::Trivec& p2);

    private:
        pga3::Trivec p1 = pga3::e123;
        pga3::Trivec p2 = pga3::e123;

        virtual std::pair<pga3::Trivec, pga3::Trivec> get_box()
            const override final;
};

Box merge_boxes(const Box& box1, const Box& box2);
Box transform_box(const Box& box, const pga3::Even& rotor, double scale = 1);

}

#endif
