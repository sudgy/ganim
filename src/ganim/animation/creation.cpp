#include "creation.hpp"

#include "ganim/object/bases/object.hpp"
#include "ganim/object/bivector.hpp"

#include "fading.hpp"

namespace ganim {

void create(SceneBase& scene, ObjectPtr<Object> object, AnimationArgs args)
{
    auto anim = Animation(scene, object, args);
    anim.get_starting_object().set_draw_fraction(0);
    anim.get_ending_object().set_draw_fraction(1);
    anim.at_end([&object = *object]{
        object.set_creating(false);
    });
    object->add_updater(std::move(anim), true);
    object->set_visible(true);
    object->set_creating(true);
}

void noise_create(
    SceneBase& scene,
    ObjectPtr<Object> object,
    double noise_scale,
    AnimationArgs args
)
{
    auto anim = Animation(scene, object, args);
    anim.get_starting_object().set_draw_fraction(0);
    anim.get_ending_object().set_draw_fraction(1);
    anim.at_end([&object = *object]{
        object.set_noise_creating(0.0);
    });
    object->add_updater(std::move(anim), true);
    object->set_visible(true);
    object->set_noise_creating(noise_scale);
}

void write(
    SceneBase& scene,
    ObjectPtr<Object> object,
    AnimationArgs args
)
{
    args.rate_function = rf::linear;
    noise_create(scene, std::move(object), 0.25, args);
}

void create_bivector(
    SceneBase& scene,
    ObjectPtr<Bivector> bivector,
    AnimationArgs args
)
{
    fade_in(scene, bivector->get_inside(), args);
    create(scene, bivector->get_outside(), args);
}

}
