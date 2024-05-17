#ifndef GANIM_DRAWABLE_HPP
#define GANIM_DRAWABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Drawable "Drawable" class
 */

#include "ganim/gl/shader.hpp"
#include "ganim/scene/camera.hpp"
#include "ganim/color.hpp"

namespace ganim {
    /** @brief The base class for all drawable objects */
    class Drawable {
        public:
            /** @brief Draw this object onto the screen
             *
             * @param camera The camera being used to draw onto the screen.  You
             * should use this to get the x and y scale values and the camera's
             * position/orientation.
             */
            virtual void draw(const Camera& camera)=0;
            /** @brief Draw the outline of this object onto the screen
             *
             * Drawing the object and the outline are separated so that you can
             * draw several outlines of the objects in a group before drawing
             * the objects themselves.
             *
             * @param camera The camera being used to draw onto the screen.  You
             * should use this to get the x and y scale values and the camera's
             * position/orientation.
             */
            virtual void draw_outline(const Camera& camera)=0;
            /** @brief See whether this object is visible */
            virtual bool is_visible() const=0;
            /** @brief Set the outline color and thickness to use when drawing
             * the outline of this object
             *
             * Use a value of 0.0 for the thickness to disable the outline.
             *
             * @param color The color of the outline.
             * @param thickness The width, in ganim units, of the outline.
             */
            virtual void set_outline(const Color& color, double thickness)=0;
            virtual void invalidate_outline()=0;
    };
}

#endif
