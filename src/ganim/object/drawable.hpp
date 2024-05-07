#ifndef GANIM_DRAWABLE_HPP
#define GANIM_DRAWABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Drawable "Drawable" class
 */

#include "ganim/gl/shader.hpp"
#include "ganim/scene/camera.hpp"

namespace ganim {
    /** @brief The base class for all drawable objects */
    class Drawable {
        public:
            /** @brief Draw this object onto the screen */
            virtual void draw(const Camera& camera)=0;
            /** @brief See whether this object is visible */
            virtual bool is_visible() const=0;
    };
}

#endif
