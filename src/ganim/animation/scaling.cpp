#include "scaling.hpp"

#include "ganim/object/bases/object.hpp"

void ganim::grow_from_point(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    ScalingArgs args
)
{
    auto new_args = AnimationArgs(args.duration, args.rate_function);
    auto anim = Animation(scene, std::move(object), new_args);
    auto about_point = args.about_point.value_or(
            pga3_to_vga3(object->get_origin()));
    anim.get_starting_object().scale(1e-5, about_point);
    object->set_visible(true);
    object->scale(1e-5, about_point);
    object->add_updater(std::move(anim), true);
}

void ganim::shrink_to_point(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
    ScalingArgs args
)
{
    auto new_args = AnimationArgs(args.duration, args.rate_function);
    auto anim = Animation(scene, std::move(object), new_args);
    auto about_point = args.about_point.value_or(
            pga3_to_vga3(object->get_origin()));
    anim.get_ending_object().scale(1e-5, about_point);
    anim.at_end([&object = *object, about_point]{
        object.scale(1e5, about_point);
        object.set_visible(false);
    });
    object->add_updater(std::move(anim), true);
}
