#ifndef GANIM_ANIMATION_HPP
#define GANIM_ANIMATION_HPP

/** @file
 * @brief Contains the @ref ganim::Animation "Animation" class and things
 * related to it
 */

#include <cmath>
#include <memory>
#include <concepts>
#include <iostream>

#include "ganim/object/animatable.hpp"
#include "ganim/object/group.hpp"

#include "ganim/rate_functions.hpp"

namespace ganim {

/** @brief A concept specifying what types of objects can be animated
 *
 * For a type to be animatable, it must derive from @ref ganim::Animatable
 * "Animatable" and have two functions: `anim_copy`, which creates a copy of
 * this object in a `std::unique_ptr` for use in animations, and `interpolate`,
 * which takes in two objects and a `double` and interpolates the object between
 * the two input objects.  The reason for using `anim_copy` rather than a copy
 * constructor is that these copies are never drawn, so subclasses that contain
 * a lot of data for drawing (such as OpenGL handles) don't need to create
 * copies of those when getting animated.
 *
 * Interestingly, `anim_copy` doesn't have to return an object of the same type,
 * and `interpolate` doesn't have to take in objects of the same type either!
 * `anim_copy` just needs to return a unique pointer of something that can be
 * converted to an `Animatable`, and `interpolate` only needs to be able to
 * handle whatever `anim_copy` returns.  This is to allow subclasses of
 * animatable classes to still be animatable.  Here are a few examples:
 *  - Class `A` defines `anim_copy` to return a `std::unique_ptr<A>`, and
 *    `interpolate` takes in two `A`s.  This class can be animated like normal.
 *  - Class `B` inherits from `A` and doesn't define either function.  This
 *    class can animated, although you can only ever animate the data in `A`.
 *  - Class `C` inherits from `A` and defines a new `anim_copy` that returns a
 *    `std::unique_ptr<C>`, but doesn't define a new interpolate.  The animation
 *    will work with `C` objects, but will only be able to interpolate the data
 *    in `A`.  This can be useful when you define a new object type that
 *    contains no new data, but it has new functions that manipulate the data in
 *    `A`.
 *
 * Note that a major exception to the second case is groups.  Groups copy and
 * interpolate significantly differently, and if you do an animation on a group
 * but using a base class' functions, the results will be incorrect.
 */
template <typename T>
concept animatable = std::derived_from<T, Animatable> and
requires(T& a, const T& b, const T& c, double t) {
    {b.anim_copy()} -> std::convertible_to<std::unique_ptr<Animatable>>;
    {a.interpolate(*b.anim_copy(), *c.anim_copy(), t)};
};

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
        decltype(*std::declval<T>().anim_copy())>;
    public:
        /** @brief constructor
         *
         * @param object The object to be animated
         * @param args The parameters for this animation
         */
        explicit Animation(T& object, AnimationArgs args = AnimationArgs())
        :   M_rate_function(std::move(args.rate_function)),
            M_object(object)
        {
            if constexpr (std::convertible_to<
                    std::unique_ptr<Group>, std::shared_ptr<copy_type>>)
            {
                if (auto group = object.as_group()) {
                    M_starting_object = group->anim_copy();
                    M_ending_object = group->anim_copy();
                }
                else {
                    M_starting_object = object.anim_copy();
                    M_ending_object = object.anim_copy();
                }
            }
            else {
                M_starting_object = object.anim_copy();
                M_ending_object = object.anim_copy();
            }
            auto fps = object.get_fps();
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
            M_object.interpolate(
                *M_starting_object,
                *M_ending_object,
                M_rate_function(t)
            );
            if (M_animation_progress == M_animation_time) {
                M_animation_time = 0;
                if (M_at_end) M_at_end();
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

    private:
        std::function<double(double)> M_rate_function;
        std::function<void()> M_at_end;
        T& M_object;
        // TODO: When switching the library to C++23, use move_only_function so
        // that these can be unique_ptrs
        std::shared_ptr<copy_type> M_starting_object;
        std::shared_ptr<copy_type> M_ending_object;
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
auto& animate(animatable auto& object, AnimationArgs args = AnimationArgs())
{
    auto anim = Animation(object, args);
    auto& result = anim.get_ending_object();
    object.add_updater(std::move(anim));
    return result;
}

}

#endif
