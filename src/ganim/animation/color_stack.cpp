#include "color_stack.hpp"

void ganim::push_color(
    SceneBase& scene,
    ObjectPtr<Object> object,
    Color color,
    AnimationArgs args
)
{
    auto anim = Animation(scene, object, args);
    auto start_color = object->get_color();
    anim.get_starting_object().push_color(start_color);
    object->push_color(start_color);
    anim.get_ending_object().push_color(color);
    object->add_updater(std::move(anim), true);
}

void ganim::pop_color(
    SceneBase& scene,
    ObjectPtr<Object> object,
    AnimationArgs args
)
{
    auto anim = Animation(scene, object, args);
    auto start_color = object->get_color();
    object->pop_color();
    auto end_color = object->get_color();
    object->push_color(start_color);

    anim.get_ending_object().set_color(end_color);
    anim.at_end([object]{
        object->pop_color();
    });
    object->add_updater(std::move(anim), true);
}
