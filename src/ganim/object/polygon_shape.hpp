#ifndef GANIM_OBJECT_POLYGON_SHAPE_HPP
#define GANIM_OBJECT_POLYGON_SHAPE_HPP

#include "ganim/object/shape.hpp"

namespace ganim {
    class PolygonShape : public Shape {
        public:
            explicit PolygonShape(const std::vector<vga2::Vec>& vertices);
            explicit PolygonShape(const std::vector<pga2::Bivec>& vertices);
    };
    inline ObjectPtr<PolygonShape> make_polygon_shape(
        const std::vector<vga2::Vec>& vertices
    )
    {
        return ObjectPtr<PolygonShape>(vertices);
    }
    inline ObjectPtr<PolygonShape> make_polygon_shape(
        const std::vector<pga2::Bivec>& vertices
    )
    {
        return ObjectPtr<PolygonShape>(vertices);
    }
}

#endif
