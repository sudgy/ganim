#include "scaling.hpp"

#include "ganim/object/bases/object.hpp"

void ganim::grow_from_point(
    SceneBase& scene,
    ObjectPtr<Object> object,
    ScalingArgs args
)
{
    auto new_args = AnimationArgs(args.duration, args.rate_function);
    auto anim = Animation(scene, object, new_args);
    auto about_point = args.about_point.value_or(
            pga3_to_vga3(object->get_origin()));
    anim.get_starting_object().scale(1e-5, about_point);
    object->set_visible(true);
    object->scale(1e-5, about_point);
    object->add_updater(std::move(anim), true);
}

void ganim::shrink_to_point(
    SceneBase& scene,
    ObjectPtr<Object> object,
    ScalingArgs args
)
{
    auto new_args = AnimationArgs(args.duration, args.rate_function);
    auto anim = Animation(scene, object, new_args);
    auto about_point = args.about_point.value_or(
            pga3_to_vga3(object->get_origin()));
    anim.get_ending_object().scale(1e-5, about_point);
    anim.at_end([&object = *object, about_point]{
        object.scale(1e5, about_point);
        object.set_visible(false);
    });
    object->add_updater(std::move(anim), true);
}

void ganim::shrink_in(
    SceneBase& scene,
    ObjectPtr<Object> object,
    GrowArgs args
)
{
    auto new_args = AnimationArgs(args.duration, args.rate_function);
    auto anim = Animation(scene, object, new_args);
    auto about_point = args.about_point.value_or(
            pga3_to_vga3(object->get_origin()));
    anim.get_starting_object().scale(args.scale_factor, about_point);
    anim.get_starting_object().set_opacity(0);
    anim.each_frame([object](double t) {
        object->set_opacity(t*t);
    });
    object->set_visible(true);
    object->scale(args.scale_factor, about_point);
    object->set_opacity(0);
    object->add_updater(std::move(anim), true);
}
