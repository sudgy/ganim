#ifndef GANIM_ANIMATION_HPP
#define GANIM_ANIMATION_HPP

/** @file
 * @brief Contains the @ref ganim::Animation "Animation" class and things
 * related to it
 */

#include <cmath>
#include <concepts>
#include <iostream>

#include "ganim/object/bases/animatable.hpp"
#include "ganim/object/bases/group.hpp"
#include "ganim/scene/base.hpp"

#include "ganim/rate_functions.hpp"

namespace ganim {

class SceneBase;

/** @brief A concept specifying what types of objects can be animated
 */
template <typename T>
concept animatable = std::derived_from<T, Animatable>;

/** @brief A struct to be passed to animations that specifies how the animation
 * should be run
 *
 * You can use designated initializers to emulate named parameters to
 * animations.  The defaults are a duration of 1 second and the rate function
 * @ref ganim::rf::smoothererstep "smoothererstep".
 */
struct AnimationArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
};

/** @brief The main animation class
 *
 * All animations will eventually be run through this class.  Note that it is
 * relatively low-level, and you're intended to define more high-level
 * animations using this one.  Unless you're creating your own animation, you
 * probably will not need to use this class directly.
 *
 * To make an animation, create an object of this type while passing in the
 * object that you want to be animated.  Then you can modify the starting state
 * and ending state with @ref get_starting_object and @ref get_ending_object.
 * The object will then interpolate between those two states.  If you want
 * something to run at the end of the animation (such as making the object not
 * visible for animations that conceptually make an object disappear), use @ref
 * at_end.
 *
 * @tparam T The type of object getting animated.  See @ref ganim::animatable
 * "animatable".
 */
template <animatable T>
class Animation {
    using copy_type = std::remove_reference_t<
        decltype(*std::declval<T>().polymorphic_copy())>;
    public:
        /** @brief constructor
         *
         * @param scene The scene the animation is taking place in.
         * @param object The object to be animated
         * @param args The parameters for this animation
         */
        explicit Animation(
            SceneBase& scene,
            ObjectPtr<T> object,
            AnimationArgs args = AnimationArgs()
        )
        :   M_rate_function(std::move(args.rate_function)),
            M_object(object),
            M_scene(scene)
        {
            if (object->is_animating()) {
                throw std::invalid_argument(
                    "Attempting to animate an object that is already being "
                    "animated."
                );
            }
            else {
                object->set_animating(true);
            }
            scene.add(object);
            M_starting_object = object->polymorphic_copy();
            M_ending_object = object->polymorphic_copy();
            auto fps = object->get_fps();
            if (fps == -1) {
                throw std::logic_error("An animation was run without setting "
                    "the fps.  Did you forget to add something to the scene?");
            }
            M_animation_time = std::round(args.duration * fps);
        }

        /** @brief Updates the animation.  Do not call this yourself. */
        bool operator()()
        {
            M_starting_object->update();
            M_ending_object->update();
            ++M_animation_progress;
            auto t = static_cast<double>(M_animation_progress)
                / M_animation_time;
            M_object->interpolate(
                *M_starting_object,
                *M_ending_object,
                M_rate_function(t)
            );
            if (M_animation_progress == M_animation_time) {
                M_animation_time = 0;
                if (M_at_end) M_at_end();
                for (auto& object : M_animation_objects) {
                    object->set_visible(false);
                }
                M_object->set_animating(false);
                return false;
            }
            return true;
        }

        /** @brief Get the starting object.
         *
         * The animation will make the object start interpolating from this
         * object.
         */
        copy_type& get_starting_object() {return *M_starting_object;}
        /** @brief Get the ending object.
         *
         * The animation will make the object interpolate to become this object.
         */
        copy_type& get_ending_object() {return *M_ending_object;}
        /** @brief Run something at the end of the animation */
        void at_end(std::function<void()> func)
        {
            M_at_end = std::move(func);
        }
        void add_animation_object(ObjectPtr<Object> object)
        {
            if (!object.get()) {
                throw std::invalid_argument(
                        "Animation objects must be non-null.");
            }
            object->set_visible(true);
            M_scene.add(object);
            M_animation_objects.emplace_back(object);
        }

    private:
        std::move_only_function<double(double)> M_rate_function;
        std::move_only_function<void()> M_at_end;
        ObjectPtr<T> M_object;
        SceneBase& M_scene;
        ObjectPtr<copy_type> M_starting_object = nullptr;
        ObjectPtr<copy_type> M_ending_object = nullptr;
        std::vector<ObjectPtr<Object>> M_animation_objects;
        int M_animation_progress = 0;
        int M_animation_time = 0;
};

/** @brief A generic animation for changing an object from its initial state to
 * some other state
 *
 * This will create an @ref ganim::Animation Animation and then return a
 * reference to the animation's ending object.  You can then modify this ending
 * object as you wish, and the object will then animate becoming that object.
 * For example, you can run `animate(object).shift(2*e1)` to animate shifting
 * two units to the right.
 */
auto& animate(
    SceneBase& scene,
    ObjectPtr<animatable auto> object,
    AnimationArgs args = AnimationArgs()
)
{
    auto anim = Animation(scene, object, args);
    auto& result = anim.get_ending_object();
    object->add_updater(std::move(anim), true);
    return result;
}

}

#endif
