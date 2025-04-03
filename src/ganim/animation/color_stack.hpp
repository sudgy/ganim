#ifndef GANIM_ANIMATION_COLOR_STACK_HPP
#define GANIM_ANIMATION_COLOR_STACK_HPP

/** @file
 * @brief Contains animations for manipulating the color stack
 */

#include "animation.hpp"

namespace ganim {

void push_color(
    SceneBase& scene,
    ObjectPtr<Object> object,
    Color color,
    AnimationArgs args = AnimationArgs()
);

void pop_color(
    SceneBase& scene,
    ObjectPtr<Object> object,
    AnimationArgs args = AnimationArgs()
);

}

#endif
