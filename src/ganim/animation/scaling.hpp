#ifndef GANIM_ANIMATION_SCALING_HPP
#define GANIM_ANIMATION_SCALING_HPP

#include "animation.hpp"

namespace ganim {

struct ScalingArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
    std::optional<vga3::Vec> about_point = {};
};

void grow_from_point(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    ScalingArgs args = ScalingArgs()
);

void shrink_to_point(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    ScalingArgs args = ScalingArgs()
);

}

#endif
