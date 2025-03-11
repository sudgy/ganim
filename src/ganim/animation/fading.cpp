#include "fading.hpp"

#include "ganim/object/bases/object.hpp"

namespace ganim {

void fade_in(SceneBase& scene, ObjectPtr<Object> object, AnimationArgs args)
{
    args.rate_function = rf::linear;
    object->set_opacity(1);
    auto anim = Animation(scene, object, args);
    anim.get_starting_object().set_opacity(0);
    object->set_visible(true);
    object->set_opacity(0);
    object->add_updater(std::move(anim), true);
}

void fade_out(SceneBase& scene, ObjectPtr<Object> object, AnimationArgs args)
{
    args.rate_function = rf::linear;
    object->set_opacity(1);
    auto anim = Animation(scene, object, args);
    anim.get_ending_object().set_opacity(0);
    anim.at_end([&object = *object]{
        object.set_opacity(1);
        object.set_visible(false);
    });
    object->add_updater(std::move(anim), true);
}

}
