#ifndef GANIM_OBJECT_OBJECT_HPP
#define GANIM_OBJECT_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Object "Object" class
 */

#include "transformable.hpp"

#include "ganim/color.hpp"

namespace ganim {
    /** @brief The base class for objects that have some kind of extent
     */
    class Object : public Transformable {
        public:
            Object& set_color(Color color);
            Object& set_color_with_alpha(Color color);
            Object& set_opacity(double opacity);
            Color get_color() const {return M_color;}

            GANIM_TRANSFORMABLE_CHAIN_DECLS(Object);

        private:
            virtual void transformable_on_animate() override final;
            virtual void transformable_on_animation_start() override final;
            virtual void transformable_update_animation(double t)override final;
            virtual void transformable_on_animation_end() override final;
            /** @brief Called by @ref transformable_on_animate */
            virtual void object_on_animate() {}
            /** @brief Called by @ref transformable_on_animation_start */
            virtual void object_on_animation_start() {}
            /** @brief Called by @ref transformable_update_animation */
            virtual void object_update_animation(double t) {(void)t;}
            /** @brief Called by @ref transformable_on_animation_end */
            virtual void object_on_animation_end() {}

            Color M_color = {255, 255, 255, 255};
            Color M_starting_color;
            Color M_ending_color;
    };
}

#define GANIM_OBJECT_CHAIN_DECLS(Type) \
    GANIM_TRANSFORMABLE_CHAIN_DECLS(Type) \
    Type& set_color(Color color) \
        {Object::set_color(color); return *this;} \
    Type& set_color_with_alpha(Color color) \
        {Object::set_color_with_alpha(color); return *this;} \
    Type& set_opacity(double opacity) \
        {Object::set_opacity(opacity); return *this;}

#endif
