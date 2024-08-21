#ifndef GANIM_ANIMATION_CREATION_HPP
#define GANIM_ANIMATION_CREATION_HPP

/** @file
 * @brief Contains animations related to creating objects.
 */

#include "animation.hpp"

namespace ganim {

class Object;

/** @brief An animation to "create" objects.
 *
 * "Creating" an object is setting its draw fraction continuously from zero to
 * one.  This will automatically make the object visible.
 */
void create(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    AnimationArgs args = AnimationArgs()
);

/** @brief An animation to "create" objects, but with a bit of randomness thrown
 * in.
 *
 * "Creating" an object is setting its draw fraction continuously from zero to
 * one, but with the draw fraction varying from pixel to pixel with a noise
 * function.  This will automatically make the object visible.
 *
 * @param scene The scene the animation is taking place in.
 * @param object The object being animated.
 * @param noise_scale How much the noise will affect the draw fraction.  Draw
 * fractions go from 0 to 1, and the noise scale will widen this range (but
 * internally it will get scaled back to 0 to 1).  For example, if noise_scale
 * is 0.5, the draw fraction will instead go from -0.5 to 1.5, and the noise can
 * cause the draw fraction at a particular pixel to be changed by up to 0.5 in
 * either direction.
 * @param args The @ref AnimationArgs affecting this animation.
 */
void noise_create(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    double noise_scale,
    AnimationArgs args = AnimationArgs()
);

}

#endif
