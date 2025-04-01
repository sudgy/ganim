#ifndef GANIM_OBJECT_POINT_HPP
#define GANIM_OBJECT_POINT_HPP

#include "ganim/object/polygon_shape.hpp"

namespace ganim {
    class Point : public PolygonShape {
        public:
            explicit Point(vga2::Vec p, double radius = 0.05);
            explicit Point(pga2::Bivec p, double radius = 0.05);
    };
    inline ObjectPtr<Point> make_point(vga2::Vec p, double radius = 0.05)
    {
        return ObjectPtr<Point>(p, radius);
    }
    inline ObjectPtr<Point> make_point(pga2::Bivec p, double radius = 0.05)
    {
        return ObjectPtr<Point>(p, radius);
    }
}

#endif
