#ifndef GANIM_OBJECT_POLYGON_SHAPE_HPP
#define GANIM_OBJECT_POLYGON_SHAPE_HPP

#include "ganim/object/shape.hpp"

namespace ganim {
    class PolygonShape : public Shape {
        public:
            explicit PolygonShape(const std::vector<vga2::Vec>& vertices);
            explicit PolygonShape(const std::vector<pga2::Bivec>& vertices);
    };
}

#endif
