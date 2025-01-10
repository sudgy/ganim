#ifndef GANIM_ANIMATION_INDICATE_HPP
#define GANIM_ANIMATION_INDICATE_HPP

#include "animation.hpp"

namespace ganim {

struct IndicateAnimationArgs {
    /** @brief The duration, in seconds, that an animation should be run */
    double duration = 1.0;
    /** @brief The rate function to use for this animation */
    std::function<double(double)> rate_function = rf::smoothererstep;
    double scale_factor = 1.15;
};

void indicate(
    SceneBase& scene,
    ObjectPtr<Object> object,
    IndicateAnimationArgs args = IndicateAnimationArgs()
);

}

#endif
