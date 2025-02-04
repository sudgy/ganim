#ifndef GANIM_OBJECT_ANIMATABLE_HPP
#define GANIM_OBJECT_ANIMATABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Animatable "Animatable" class
 */

#include <functional>
#include <map>
#include "ganim/object/object_ptr.hpp"

#include "updatable.hpp"

namespace ganim {
    class Group;
    /** @brief Represents any kind of thing that can be animated
     *
     * This class doesn't actually implement any animations itself.  It is
     * basically an @ref Updatable with a couple extra utilities.  To actually
     * animate things, see @ref ganim::Animation "Animation".
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
            /** @brief Set whether this object is currently being animated.
             *
             * You probably shouldn't call this yourself.  It's used to prevent
             * multiple animations happening at once on the same object.
             */
            virtual void set_animating(bool animating) {M_animating = animating;}
            /** @brief See whether this object is currently being animated.
             *
             * It's used to prevent multiple animations happening at once on the
             * same object.
             */
            virtual bool is_animating() const {return M_animating;}

            /** @brief Copy the object polymorphically.
             *
             * You may notice that this object is not virtual.  This is
             * intentional!  C++ can't do covariant return types with smart
             * pointers, so you have to use a workaround instead.  There's a
             * private virtual function called @ref polymorphic_copy_impl that
             * you must override with a raw pointer that allocates using `new`
             * (I'm sorry).  This function will call that function and wrap it
             * in an ObjectPtr.
             *
             * In subclasses, define both this and `polymorphic_copy_impl` with
             * your subclass in the return type and follow this pattern.  You'll
             * have to use @ref ObjectPtr::from_new in polymorphic_copy.
             */
            ObjectPtr<Animatable> polymorphic_copy() const;
            virtual void interpolate(
                const Animatable& start,
                const Animatable& end,
                double t
            );

        private:
            /** @brief Implementation of copying objects polymorphically.
             *
             * To support this feature in a subclass, you must override this
             * function.  For more details, see @ref
             * Animatable::polymorphic_copy.
             */
            virtual Animatable* polymorphic_copy_impl() const;
            int M_fps = -1;
            bool M_animating = false;
    };
}

#endif
