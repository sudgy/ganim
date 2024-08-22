#ifndef GANIM_UTIL_BOUNDING_BOX_HPP
#define GANIM_UTIL_BOUNDING_BOX_HPP

#include "ganim/ga/vga3.hpp"
#include "ganim/ga/pga3.hpp"

namespace ganim {

struct Box {
    vga3::Vector p1 = {0, 0, 0};
    vga3::Vector p2 = {0, 0, 0};
    vga3::Vector get_center() const;
    vga3::Vector get_left() const;
    vga3::Vector get_right() const;
    vga3::Vector get_up() const;
    vga3::Vector get_down() const;
    vga3::Vector get_out() const;
    vga3::Vector get_in() const;
    vga3::Vector get_upper_left() const;
    vga3::Vector get_upper_right() const;
    vga3::Vector get_lower_left() const;
    vga3::Vector get_lower_right() const;
};

Box merge_boxes(const Box& box1, const Box& box2);
Box transform_box(const Box& box, const pga3::Even& rotor, double scale = 1);

}

#endif
