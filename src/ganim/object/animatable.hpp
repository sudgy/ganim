#ifndef GANIM_OBJECT_ANIMATABLE_HPP
#define GANIM_OBJECT_ANIMATABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Animatable "Animatable" class
 */

#include <functional>
#include <map>

namespace ganim {
    /** @brief Represents any kind of thing that can be animated/updated
     *
     * It has two main roles: to do animations, and to do updates.  Updates come
     * along for the ride because they are necessary for animations to work.  To
     * make your own animatable object, derive from this one and overload @ref
     * on_animation_start, @ref update_animation, and @ref on_animation_end.
     *
     * All of the @ref animate functions return `*this` to allow for chaining.
     * If you want the chaining to work on subclasses, use the @ref
     * GANIM_ANIMATABLE_CHAIN_DECLS macro.
     */
    class Animatable {
        public:
            Animatable()=default;
            virtual ~Animatable()=default;
            /** @brief Set the fps for calculating how many frames animations
             * will take
             *
             * Note that calling @ref animate before this will throw an
             * exception!
             */
            void set_fps(int fps);
            /** @brief Start an animation
             *
             * This will just call @ref animate(double,
             * std::function<double(double)>) with a duration of 1 and the rate
             * function set to smoothererstep.
             *
             * @return `*this`
             */
            Animatable& animate();
            /** @brief Start an animation
             *
             * This will just call @ref animate(double,
             * std::function<double(double)>) with the given duration and the
             * rate function set to smoothererstep.
             *
             * @return `*this`
             */
            Animatable& animate(double duration);
            /** @brief Start an animation
             *
             * This will just call @ref animate(double,
             * std::function<double(double)>) with a duration of 1 and the given
             * rate function.
             *
             * @return `*this`
             */
            Animatable& animate(std::function<double(double)> rate_func);
            /** @brief Start an animation
             *
             * @param duration The duration, in seconds, of this animation.
             * @param rate_func The rate function used to specify how fast the
             * animation proceeds at different points.
             *
             * @return `*this`
             */
            Animatable& animate(
                double duration,
                std::function<double(double)> rate_func
            );
            /** @brief Returns true if the object is currently preparing for an
             * animation
             */
            bool starting_animation() const;
            /** @brief Returns true if the object is currently in an animation
             */
            bool in_animation() const;

            /** @brief Add an updater
             *
             * @param updater A function to be called every frame.  It accepts
             * two kinds of input functions: A function that returns void, and a
             * function that returns something convertible to a boolean.  In the
             * void case, the updater will be run with every call to @ref
             * update.  In the boolean case, it will run with every call to @ref
             * update, but if the updater returns false the updater will be
             * removed.
             *
             * @return A handle to the updater that can be passed to @ref
             * remove_updater.
             */
            template <typename F>
            int add_updater(F&& updater)
            {
                if constexpr (std::is_convertible_v<
                        decltype(updater()), bool>) {
                    return add_updater_bool(std::forward<F>(updater));
                }
                else {
                    return add_updater_void(std::forward<F>(updater));
                }
            }
            /** @brief Remove an updater
             *
             * @param updater_handle The handle to the updater to remove, as
             * returned by @ref add_updater.
             */
            void remove_updater(int updater_handle);
            /** @brief Remove all updaters
             *
             * Note that because animations use updaters as well, and stopping
             * an animation in the middle of it can break things, this will
             * throw an exception if an animation is currently running.
             */
            void clear_updaters();
            /** @brief Update the object
             *
             * This will call all the updaters, including ones created by
             * animations, and remove any that return false.
             */
            void update();

        private:
            /** @brief Called when @ref animate is called */
            virtual void on_animate()=0;
            /** @brief Called right before the animation starts */
            virtual void on_animation_start()=0;
            /** @brief Called once each frame an animation is running
             *
             * @param t The progress through the animation.  Note that the rate
             * function will have already been applied to this value, so no need
             * to do that yourself.  Also, unless a strange rate function is
             * being used, this value will never be zero!  The first time an
             * animation runs is on the first frame after the animation is
             * called, not the zeroth frame.  The state corresponding to t = 0
             * is assumed to be the state before the animation even started
             * running.
             */
            virtual void update_animation(double t)=0;
            /** @brief Called when an animation is ending */
            virtual void on_animation_end()=0;
            int add_updater_void(std::function<void()> updater);
            int add_updater_bool(std::function<bool()> updater);
            std::map<int, std::function<bool()>> M_updaters;
            int M_fps = -1;
            int M_animation_progress = 0;
            int M_animation_time = 0;
    };
}

/** @brief Defines the @ref ganim::Animatable "Animatable" chainable functions
 * in a subclass to allow chaining in the subclass as well
 *
 * To use it, just call this macro in the public part of the class declaration,
 * where the Type parameter is the name of the subclass.
 */
#define GANIM_ANIMATABLE_CHAIN_DECLS(Type) \
    Type& animate() \
        {Animatable::animate(); return *this;} \
    Type& animate(double duration) \
        {Animatable::animate(duration); return *this;} \
    Type& animate(std::function<double(double)> rate_func) \
        {Animatable::animate(std::move(rate_func)); return *this;} \
    Type& animate(double duration, std::function<double(double)> rate_func) \
        {Animatable::animate(duration, std::move(rate_func)); return *this;} \

#endif
