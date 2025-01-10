#ifndef GANIM_ANIMATION_FADING_HPP
#define GANIM_ANIMATION_FADING_HPP

/** @file
 * @brief Contains animations related to fading objects in and out.
 */

#include "animation.hpp"

namespace ganim {

/** @brief An animation to fade in an object.
 *
 * This will automatically make the object visible.
 */
void fade_in(
    SceneBase& scene,
    ObjectPtr<Object> object,
    AnimationArgs args = AnimationArgs()
);
template <typename... Ts> requires(
    sizeof...(Ts) > 1 and
    (std::is_base_of_v<
     Object, typename std::remove_reference_t<Ts>::element_type> and ...)
)
void fade_in(
    SceneBase& scene,
    Ts&&... objects
)
{
    (fade_in(scene, std::forward<Ts>(objects)), ...);
}
template <typename... Ts> requires(
    sizeof...(Ts) > 1 and
    (std::is_base_of_v<
     Object, typename std::remove_reference_t<Ts>::element_type> and ...)
)
void fade_in(
    SceneBase& scene,
    AnimationArgs args,
    Ts&&... objects
)
{
    (fade_in(scene, std::forward<Ts>(objects)), ...);
}
/** @brief An animation to fade out an object.
 *
 * This automatically makes the object not visible at the end, and it will set
 * its opacity back to its original value before it started fading out.
 */
void fade_out(
    SceneBase& scene,
    ObjectPtr<Object> object,
    AnimationArgs args = AnimationArgs()
);
template <typename... Ts> requires(
    sizeof...(Ts) > 1 and
    (std::is_base_of_v<
     Object, typename std::remove_reference_t<Ts>::element_type> and ...)
)
void fade_out(
    SceneBase& scene,
    Ts&&... objects
)
{
    (fade_out(scene, std::forward<Ts>(objects)), ...);
}
template <typename... Ts> requires(
    sizeof...(Ts) > 1 and
    (std::is_base_of_v<
     Object, typename std::remove_reference_t<Ts>::element_type> and ...)
)
void fade_out(
    SceneBase& scene,
    AnimationArgs args,
    Ts&&... objects
)
{
    (fade_out(scene, std::forward<Ts>(objects)), ...);
}

}

#endif
