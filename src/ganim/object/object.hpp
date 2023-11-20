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
            Color M_color = {255, 255, 255, 255};
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
