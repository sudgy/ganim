#ifndef GANIM_ANIMATION_SCALING_HPP
#define GANIM_ANIMATION_SCALING_HPP

#include "animation.hpp"

namespace ganim {

/** @brief An "extension" of AnimationArgs for the scaling animations */
struct ScalingArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
    /** @brief The point to scale around.
     *
     * If it is not set, it will use the origin of the object.
     */
    std::optional<vga3::Vec> about_point = {};
};

/** @brief An "extension" of AnimationArgs for the @ref grow_in animation */
struct GrowArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
    /** @brief The point to scale around.
     *
     * If it is not set, it will use the origin of the object.
     */
    std::optional<vga3::Vec> about_point = {};
    /** @brief The amount the object should start out scaled
     */
    double scale_factor = 2;
};

/** @brief Create an object by scaling it up from nothing from a certain point.
 *
 * This will automatically make the object visible.
 */
void grow_from_point(
    SceneBase& scene,
    ObjectPtr<Object> object,
    ScalingArgs args = ScalingArgs()
);


/** @brief Scale an object down to zero scale to a particular point.
 *
 * This will automatically make the object not visible at the end, and it will
 * reset its scale back to what it was before the animation was started.
 */
void shrink_to_point(
    SceneBase& scene,
    ObjectPtr<Object> object,
    ScalingArgs args = ScalingArgs()
);

/** @brief Add an object to the scene by having it start out big and transparent
 * and then shrink to its normal size while becoming opaque
 *
 * This will automatically make the object visible.
 */
void shrink_in(
    SceneBase& scene,
    ObjectPtr<Object> object,
    GrowArgs args = GrowArgs()
);

}

#endif
