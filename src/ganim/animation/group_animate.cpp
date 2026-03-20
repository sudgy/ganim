#include "group_animate.hpp"

using namespace ganim;

namespace {
    class GroupAnimator {
        public:
            GroupAnimator(
                SceneBase& scene,
                ObjectPtr<Group> group,
                std::move_only_function<
                    void(SceneBase&, ObjectPtr<Object>)> animation,
                int animation_time
            )
            :   M_scene(scene),
                M_group(std::move(group)),
                M_animation(std::move(animation)),
                M_animation_time(animation_time),
                M_size(M_group->size())
                {}

            bool operator()()
            {
                ++M_animation_progress;
                auto current = M_size * M_animation_progress / M_animation_time;
                for (int i = M_current + 1;
                     i <= current and i < M_group->size();
                     ++i)
                {
                    M_animation(M_scene, M_group[i]);
                }
                M_current = current;
                if (M_animation_progress == M_animation_time) {
                    M_group->Object::set_animating(false);
                    return false;
                }
                else return true;
            }

        private:
            SceneBase& M_scene;
            ObjectPtr<Group> M_group;
            std::move_only_function<void(SceneBase&, ObjectPtr<Object>)>
                M_animation;
            int M_animation_time;
            int M_animation_progress = -1;
            int M_size = -1;
            int M_current = -1;
    };
}

void ganim::group_animate(
    SceneBase& scene,
    ObjectPtr<Group> object,
    std::move_only_function<void(SceneBase&, ObjectPtr<Object>)> animation
)
{
    if (object->is_animating()) {
        throw std::invalid_argument(std::format(
            "Attempting to animate an object that is already being "
            "animated.  Stacktrace:\n{}", std::stacktrace::current()
        ));
    }
    else {
        object->Object::set_animating(true);
    }
    scene.add(object);
    auto fps = object->get_fps();
    if (fps == -1) {
        throw std::logic_error("An animation was run without setting "
            "the fps.  Did you forget to add something to the scene?");
    }
    auto animation_time = std::round(fps);
    auto anim = GroupAnimator(
        scene,
        object,
        std::move(animation),
        animation_time
    );
    object->add_updater(std::move(anim), true);
}
