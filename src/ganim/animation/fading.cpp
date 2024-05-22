#include "fading.hpp"

#include "ganim/object/bases/object.hpp"

namespace ganim {

void fade_in(Object& object, AnimationArgs args)
{
    args.rate_function = rf::linear;
    auto anim = Animation(object, args);
    anim.get_starting_object().set_opacity(0);
    object.set_visible(true);
    object.set_opacity(0);
    object.add_updater(std::move(anim), true);
}

void fade_out(Object& object, AnimationArgs args)
{
    args.rate_function = rf::linear;
    auto orig_opacity = object.get_color().a / 255.0;
    auto anim = Animation(object, args);
    anim.get_ending_object().set_opacity(0);
    anim.at_end([&object, orig_opacity]{
        object.set_opacity(orig_opacity);
        object.set_visible(false);
    });
    object.add_updater(std::move(anim), true);
}

}
