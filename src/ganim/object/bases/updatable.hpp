#ifndef GANIM_OBJECT_UPDATABLE_HPP
#define GANIM_OBJECT_UPDATABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Updatable "Updatable" class
 */

#include <functional>
#include <map>

namespace ganim {
    /** @brief Represents any kind of thing that can be updated
     */
    class Updatable {
        public:
            Updatable()=default;
            virtual ~Updatable()=default;

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

        private:
            int add_updater_void(std::function<void()> updater);
            int add_updater_bool(std::function<bool()> updater);
            std::map<int, std::function<bool()>> M_updaters;
    };
}

#endif
