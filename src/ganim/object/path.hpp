#ifndef GANIM_OBJECT_PATH_HPP
#define GANIM_OBJECT_PATH_HPP

#include "ganim/object/shape.hpp"

namespace ganim {
    class Path : public Shape {
        public:
            explicit Path(
                const std::vector<pga2::Bivector>& points,
                bool closed = false,
                double thickness = 0.04
            );
            explicit Path(
                const std::vector<vga2::Vector>& points,
                bool closed = false,
                double thickness = 0.04
            );
            void recreate(
                const std::vector<pga2::Bivector>& points,
                bool closed = false,
                double thickness = 0.04
            );
            void recreate(
                const std::vector<vga2::Vector>& points,
                bool closed = false,
                double thickness = 0.04
            );
    };
}

#endif
