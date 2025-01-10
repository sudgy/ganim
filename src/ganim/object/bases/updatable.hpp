#ifndef GANIM_OBJECT_UPDATABLE_HPP
#define GANIM_OBJECT_UPDATABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Updatable "Updatable" class
 */

#include <functional>
#include <map>

namespace ganim {
    /** @brief Represents any kind of thing that can be updated.
     *
     * Note that copying this class will not copy the updaters!  This is
     * intentional.  Updaters often modify the class itself, and upon copying
     * the updaters will not do what you expect them to.
     */
    class Updatable {
        public:
            Updatable()=default;
            virtual ~Updatable()=default;
            Updatable(Updatable&&)=default;
            Updatable(const Updatable&) {}
            Updatable& operator=(Updatable&&)=default;
            Updatable& operator=(const Updatable&) {return *this;}

            /** @brief Add an updater
             *
             * By default, any updater added can be removed by a call to @ref
             * clear_updaters.  If you don't want this to be allowed, set the
             * `persistent` paramater to true.  A persistent updater can only be
             * removed by it returning false or an explicit call to @ref
             * remove_updater.
             *
             * @param updater A function to be called every frame.  It accepts
             * two kinds of input functions: A function that returns void, and a
             * function that returns something convertible to a boolean.  In the
             * void case, the updater will be run with every call to @ref
             * update.  In the boolean case, it will run with every call to @ref
             * update, but if the updater returns false the updater will be
             * removed.
             *
             * @param persistent Whether the updater should be persistent, that
             * is, if it should not be removed by a call to @ref clear_updaters.
             *
             * @return A handle to the updater that can be passed to @ref
             * remove_updater.
             */
            template <typename F>
            int add_updater(F&& updater, bool persistent = false)
            {
                if constexpr (std::is_convertible_v<
                        decltype(updater()), bool>) {
                    return add_updater_bool(
                        std::forward<F>(updater),
                        persistent
                    );
                }
                else {
                    return add_updater_void(
                        std::forward<F>(updater),
                        persistent
                    );
                }
            }
            /** @brief Remove an updater
             *
             * @param updater_handle The handle to the updater to remove, as
             * returned by @ref add_updater.
             */
            void remove_updater(int updater_handle);
            /** @brief Remove all non-persistent updaters
             *
             * An updater is persistent if the persistent parameter was set when
             * @ref add_updater was called.
             */
            void clear_updaters();
            void clear_all_updaters();
            /** @brief Update the object
             *
             * This will call all of the updaters and remove any that return
             * false.
             */
            void update();

        private:
            int add_updater_void(std::move_only_function<void()>updater, bool persistent);
            int add_updater_bool(std::move_only_function<bool()>updater, bool persistent);
            std::map<int, std::pair<std::move_only_function<bool()>, bool>> M_updaters;
    };
}

#endif
