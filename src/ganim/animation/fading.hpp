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
    MaybeOwningRef<Object> object,
    AnimationArgs args = AnimationArgs()
);
/** @brief An animation to fade out an object.
 *
 * This automatically makes the object not visible at the end, and it will set
 * its opacity back to its original value before it started fading out.
 */
void fade_out(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    AnimationArgs args = AnimationArgs()
);

}

#endif
