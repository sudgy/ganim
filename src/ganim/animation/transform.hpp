#ifndef GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP
#define GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP

#include "animation.hpp"

namespace ganim {

/** @brief An "extension" of AnimationArgs for the transform animations */
struct TransformAnimationArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
    /** @brief A direction to swing towards and back during the transform.
     *
     * In addition to doing the transform animation, the transforming object
     * will move towards this direction and back again.  In particular, with a
     * rate function of f(t), it will add sin(f(t)*τ/2)*direction to whatever
     * its position is.
     */
    vga3::Vec direction = 0*vga3::e1;
    /** @brief Whether to copy the original object when transforming.
     *
     * By default, transform animations turn one object into another, and remove
     * the original object from the scene.  Set this flag to true to keep the
     * original object.
     */
    bool copy = false;
};
/** @brief Turn one object into another, based on what the objects actually look
 * like
 *
 * This will take two objects, calculate their distance transforms, and then
 * "interpolate" between the two by picking pixels from each based on the
 * distance transform and the current time in the animation.
 *
 * It will not actually modify from or to, except that from will be made not
 * visible at the start of the animation, and to will be made visible at the end
 * of the animation.
 */
void texture_transform(
    SceneBase& scene,
    ObjectPtr<Object> from,
    ObjectPtr<Object> to,
    TransformAnimationArgs args = TransformAnimationArgs()
);
/** @brief Turn one group into another by doing a texture_transform on
 * subobjects
 *
 * This will map the elements of the first group onto the elements of the second
 * group and then do a texture_transform on each pair.
 *
 * It will not actually modify from or to, except that from will be made not
 * visible at the start of the animation, and to will be made visible at the end
 * of the animation.
 */
void group_transform(
    SceneBase& scene,
    ObjectPtr<Group> from,
    ObjectPtr<Group> to,
    TransformAnimationArgs args = TransformAnimationArgs()
);
/** @brief Transform one object into another of the same type
 *
 * All this does is make a temporary object that interpolates between from and
 * to.  While I want to eventually make arbitrary objects able to interpolate
 * between each other, this currently isn't really possible, so this will
 * probably not work unless you are transforming *very* similar objects.
 *
 * It will not actually modify from or to, except that from will be made not
 * visible at the start of the animation, and to will be made visible at the end
 * of the animation.
 */
void transform(
    SceneBase& scene,
    ObjectPtr<Object> from,
    ObjectPtr<Object> to,
    TransformAnimationArgs args = TransformAnimationArgs()
);

}

#endif
