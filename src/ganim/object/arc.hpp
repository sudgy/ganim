#ifndef GANIM_OBJECT_ARC_HPP
#define GANIM_OBJECT_ARC_HPP

#include "ganim/object/path.hpp"

namespace ganim {
    class Arc : public Path {
        public:
            Arc(
                double start_angle,
                double end_angle,
                double radius,
                double thickness = 0.04
            );
    };
    /** @brief Make an Arc in an ObjectPtr.
     *
     * @see Arc::Arc
     */
    inline ObjectPtr<Arc> make_arc(
        double start_angle,
        double end_angle,
        double radius,
        double thickness = 0.04
    )
    {
        return ObjectPtr<Arc>(start_angle, end_angle, radius, thickness);
    }
}

#endif
