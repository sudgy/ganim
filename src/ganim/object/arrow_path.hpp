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
                const std::vector<pga2::Bivec>& points,
                ArrowPathArgs args = {}
            );
            explicit ArrowPath(
                const std::vector<vga2::Vec>& points,
                ArrowPathArgs args = {}
            );
            void recreate(
                std::vector<pga2::Bivec> points,
                ArrowPathArgs args = {}
            );
            void recreate(
                const std::vector<vga2::Vec>& points,
                ArrowPathArgs args = {}
            );

        private:
            ObjectPtr<Path> M_path = nullptr;
            ObjectPtr<Shape> M_tip = nullptr;
    };
    inline ObjectPtr<ArrowPath> make_arrow_path(
        const std::vector<pga2::Bivec>& points,
        ArrowPathArgs args = {}
    )
    {
        return ObjectPtr<ArrowPath>(points, args);
    }
    inline ObjectPtr<ArrowPath> make_arrow_path(
        const std::vector<vga2::Vec>& points,
        ArrowPathArgs args = {}
    )
    {
        return ObjectPtr<ArrowPath>(points, args);
    }
}

#endif
