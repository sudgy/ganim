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
            void set_color(Color color);
            void set_color_with_alpha(Color color);
            void set_opacity(double opacity);
            Color get_color() const {return M_color;}

        private:
            Color M_color = {255, 255, 255, 255};
    };
}

#endif
