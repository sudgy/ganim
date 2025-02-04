#ifndef GANIM_ANIMATION_INDICATE_HPP
#define GANIM_ANIMATION_INDICATE_HPP

#include "animation.hpp"

namespace ganim {

/** @brief An "extension" of AnimationArgs for the @ref indicate animation */
struct IndicateAnimationArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
    /** @brief How much to scale the object when indicating */
    double scale_factor = 1.15;
};

/** @brief An animation to indicate an object.
 *
 * It will take the object, turn it yellow and scale it a bit, and then undo
 * that.
 */
void indicate(
    SceneBase& scene,
    ObjectPtr<Object> object,
    IndicateAnimationArgs args = IndicateAnimationArgs()
);

}

#endif
