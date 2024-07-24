#ifndef GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP
#define GANIM_ANIMATION_TEXTURE_TRANSFORM_HPP

#include "animation.hpp"

namespace ganim {
    class SceneBase;
    class DrawableObject;
    void texture_transform(
        SceneBase& scene,
        DrawableObject& from,
        DrawableObject& to,
        AnimationArgs args = AnimationArgs()
    );
}

#endif