#include "creation.hpp"

#include "ganim/object/object.hpp"

namespace ganim {

void create(Object& object, AnimationArgs args)
{
    auto anim = Animation(object, args);
    anim.get_starting_object().set_draw_fraction(0);
    anim.get_ending_object().set_draw_fraction(1);
    object.add_updater(std::move(anim));
    object.set_visible(true);
    object.set_creating(true);
    anim.at_end([&object]{
        object.set_creating(false);
    });
}

}
