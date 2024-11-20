#ifndef GANIM_OBJECT_ARROW_PATH_HPP
#define GANIM_OBJECT_ARROW_PATH_HPP

#include "ganim/object/path.hpp"
#include "ganim/object/bases/group.hpp"

namespace ganim {
    struct ArrowPathArgs {
        double tip_size = 0.3;
        double thickness = 0.06;
    };
    class ArrowPath : public Group {
        public:
            explicit ArrowPath(
                const std::vector<pga2::Bivector>& points,
                ArrowPathArgs args = {}
            );
            explicit ArrowPath(
                const std::vector<vga2::Vector>& points,
                ArrowPathArgs args = {}
            );
            void recreate(
                std::vector<pga2::Bivector> points,
                ArrowPathArgs args = {}
            );
            void recreate(
                const std::vector<vga2::Vector>& points,
                ArrowPathArgs args = {}
            );

        private:
            std::unique_ptr<Path> M_path;
            std::unique_ptr<Shape> M_tip;
    };
}

#endif
