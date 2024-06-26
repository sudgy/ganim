#include "creation.hpp"

#include "ganim/object/bases/object.hpp"
#include "ganim/object/bases/drawable_object.hpp"

namespace ganim {

void create(Object& object, AnimationArgs args)
{
    auto anim = Animation(object, args);
    anim.get_starting_object().set_draw_fraction(0);
    anim.get_ending_object().set_draw_fraction(1);
    anim.at_end([&object]{
        object.set_creating(false);
    });
    object.add_updater(std::move(anim));
    object.set_visible(true);
    object.set_creating(true);
}

void noise_create(Object& object, double noise_scale, AnimationArgs args)
{
    auto anim = Animation(object, args);
    anim.get_starting_object().set_draw_fraction(0);
    anim.get_ending_object().set_draw_fraction(1);
    anim.at_end([&object]{
        object.set_noise_creating(0.0);
    });
    object.add_updater(std::move(anim));
    object.set_visible(true);
    object.set_noise_creating(noise_scale);
}

}
