#ifndef GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP
#define GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP

#include "animation.hpp"

namespace ganim {
    struct TransformAnimationArgs {
        /** @brief The duration, in seconds, that an animation should be run */
        double duration = 1.0;
        /** @brief The rate function to use for this animation */
        std::function<double(double)> rate_function = rf::smoothererstep;
        vga3::Vec direction = 0*vga3::e1;
        bool copy = false;
    };
    void texture_transform(
        SceneBase& scene,
        ObjectPtr<Object> from,
        ObjectPtr<Object> to,
        TransformAnimationArgs args = TransformAnimationArgs()
    );
    void group_transform(
        SceneBase& scene,
        ObjectPtr<Group> from,
        ObjectPtr<Group> to,
        TransformAnimationArgs args = TransformAnimationArgs()
    );
    // This still doesn't work in all situations yet.  I'll try to improve it
    // later.
    void transform(
        SceneBase& scene,
        ObjectPtr<Object> from,
        ObjectPtr<Object> to,
        TransformAnimationArgs args = TransformAnimationArgs()
    );
}

#endif
