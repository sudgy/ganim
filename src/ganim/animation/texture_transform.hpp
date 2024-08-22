#ifndef GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP
#define GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP

#include "animation.hpp"

namespace ganim {
    struct TransformAnimationArgs {
        /** @brief The duration, in seconds, that an animation should be run */
        double duration = 1.0;
        /** @brief The rate function to use for this animation */
        std::function<double(double)> rate_function = rf::smoothererstep;
        vga3::Vector direction = 0*vga3::e1;
    };
    class SceneBase;
    void texture_transform(
        SceneBase& scene,
        MaybeOwningRef<Object> from,
        MaybeOwningRef<Object> to,
        TransformAnimationArgs args = TransformAnimationArgs()
    );
}

#endif
