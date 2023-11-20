#ifndef GANIM_DRAWABLE_HPP
#define GANIM_DRAWABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Drawable "Drawable" class
 */

#include "object.hpp"

namespace ganim {
    /** @brief The base class for all drawable objects */
    class Drawable : public Object {
        public:
            /** @brief Draw this object onto the screen */
            virtual void draw()=0;
    };
}

#endif
