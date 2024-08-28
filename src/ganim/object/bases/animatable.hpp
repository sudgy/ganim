#ifndef GANIM_OBJECT_ANIMATABLE_HPP
#define GANIM_OBJECT_ANIMATABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Animatable "Animatable" class
 */

#include <functional>
#include <map>

#include "updatable.hpp"

namespace ganim {
    class Group;
    /** @brief Represents any kind of thing that can be animated
     *
     * This class doesn't actually implement any animations itself.  It is
     * basically an @ref Updatable with a couple extra utilities.  To actually
     * animate things, see @ref ganim::Animation "Animation".  Also, note that
     * deriving from this class is not enough to be animatable.  You must also
     * satisfy the @ref ganim::animatable "animatable" concept.
     */
    class Animatable : public Updatable {
        public:
            Animatable()=default;
            virtual ~Animatable()=default;
            /** @brief Set the fps for calculating how many frames animations
             * will take
             *
             * Note that trying to animate before calling this will throw an
             * exception!  This function is called when adding this object to a
             * scene, so in practice this means that you must add an object to a
             * scene before animating it.
             */
            virtual void set_fps(int fps);
            /** @brief Get the fps that this object will use when animating */
            int get_fps() const {return M_fps;}
            /** @brief Returns this as a @ref Group, if possible.
             *
             * Groups often need to be handled specially, and this function
             * should be faster than a dynamic cast.
             */
            virtual Group* as_group() {return nullptr;}
            virtual const Group* as_group() const {return nullptr;}
            void set_animating(bool animating) {M_animating = animating;}
            bool is_animating() const {return M_animating;}

        private:
            int M_fps = -1;
            bool M_animating = false;
    };
}

#endif
