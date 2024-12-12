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

#include "ganim/object/bases/animatable.hpp"
#include "ganim/object/bases/group.hpp"
#include "ganim/scene/base.hpp"

#include "ganim/rate_functions.hpp"
#include "ganim/maybe_owning_ref.hpp"

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

struct ObjectRemoverUpdater {
    std::unique_ptr<Object> object;
    std::unique_ptr<bool> finished;
    bool operator()()
    {
        if (*finished) {
            object.reset();
            return false;
        }
        return true;
    }
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
            MaybeOwningRef<T> object,
            AnimationArgs args = AnimationArgs()
        )
        :   M_rate_function(std::move(args.rate_function)),
            M_object(*object),
            M_scene(scene),
            M_owning(object.is_owning())
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
            scene.add_for_animation(std::move(object));
            if constexpr (std::convertible_to<
                    std::unique_ptr<Group>, std::unique_ptr<copy_type>>)
            {
                if (auto group = object->as_group()) {
                    M_starting_object = group->polymorphic_copy();
                    M_ending_object = group->polymorphic_copy();
                }
                else {
                    M_starting_object = object->polymorphic_copy();
                    M_ending_object = object->polymorphic_copy();
                }
            }
            else {
                M_starting_object = object->polymorphic_copy();
                M_ending_object = object->polymorphic_copy();
            }
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
            M_object.interpolate(
                *M_starting_object,
                *M_ending_object,
                M_rate_function(t)
            );
            if (M_animation_progress == M_animation_time) {
                M_animation_time = 0;
                M_scene.add_updater([&object = M_object, &scene = M_scene]{
                    scene.remove_for_animation(object);
                    return false;
                });
                if (M_at_end) M_at_end();
                for (auto [object, finished] : M_animation_objects) {
                    object->set_visible(false);
                    M_scene.remove(*object);
                    *finished = true;
                }
                M_object.set_animating(false);
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
        void add_animation_object(std::unique_ptr<Object> object)
        {
            if (!object) {
                throw std::invalid_argument(
                        "Animation objects must be non-null.");
            }
            object->set_visible(true);
            M_scene.remove_for_animation(*object);
            M_scene.add(*object);
            auto updater = ObjectRemoverUpdater(
                std::move(object),
                std::make_unique<bool>(false)
            );
            M_animation_objects.emplace_back(
                updater.object.get(),
                updater.finished.get()
            );
            M_scene.add_updater(std::move(updater));
        }

    private:
        std::move_only_function<double(double)> M_rate_function;
        std::move_only_function<void()> M_at_end;
        T& M_object;
        SceneBase& M_scene;
        std::unique_ptr<copy_type> M_starting_object;
        std::unique_ptr<copy_type> M_ending_object;
        std::vector<std::pair<Object*, bool*>> M_animation_objects;
        int M_animation_progress = 0;
        int M_animation_time = 0;
        bool M_owning = false;
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
    MaybeOwningRef<animatable auto> object,
    AnimationArgs args = AnimationArgs()
)
{
    auto anim = Animation(scene, std::move(object), args);
    auto& result = anim.get_ending_object();
    object->add_updater(std::move(anim), true);
    return result;
}
auto& animate(
    SceneBase& scene,
    animatable auto& object,
    AnimationArgs args = AnimationArgs()
)
{
    return animate(scene, MaybeOwningRef(object), std::move(args));
}
auto& animate(
    SceneBase& scene,
    animatable auto&& object,
    AnimationArgs args = AnimationArgs()
)
{
    return animate(scene, MaybeOwningRef(std::move(object)), std::move(args));
}

}

#endif
