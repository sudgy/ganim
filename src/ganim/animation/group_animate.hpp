#ifndef GANIM_ANIMATION_GROUP_HPP
#define GANIM_ANIMATION_GROUP_HPP

#include "animation.hpp"

namespace ganim {

void group_animate(
    SceneBase& scene,
    ObjectPtr<Group> object,
    std::move_only_function<void(SceneBase&, ObjectPtr<Object>)> animation
);

}

#endif
