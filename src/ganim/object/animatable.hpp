#ifndef GANIM_OBJECT_ANIMATABLE_HPP
#define GANIM_OBJECT_ANIMATABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Animatable "Animatable" class
 */

#include <functional>
#include <map>

namespace ganim {
    class Group;
    /** @brief Represents any kind of thing that can be animated/updated
     *
     * This class doesn't actually implement any animations itself.  It only
     * contains functions for updating/adding updaters.  To actually animate
     * things, see @ref ganim::Animation "Animation".  Also, note that deriving
     * from this class is not enough to be animatable.  You must also satisfy
     * the @ref ganim::animatable "animatable" concept.
     */
    class Animatable {
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
            void set_fps(int fps);
            /** @brief Get the fps that this object will use when animating */
            int get_fps() const {return M_fps;}

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
             * This will call all of the updaters and remove any that return
             * false.
             */
            void update();
            /** @brief Returns this as a @ref Group, if possible.
             *
             * Groups often need to be handled specially, and this function
             * should be faster than a dynamic cast.
             */
            virtual Group* as_group() {return nullptr;}
            virtual const Group* as_group() const {return nullptr;}

        private:
            int add_updater_void(std::function<void()> updater);
            int add_updater_bool(std::function<bool()> updater);
            std::map<int, std::function<bool()>> M_updaters;
            int M_fps = -1;
    };
}

#endif
