#include "object.hpp"

using namespace ganim;

Object& Object::set_color(Color color)
{
    color.a = M_color.a;
    return set_color_with_alpha(color);
}

Object& Object::set_color_with_alpha(Color color)
{
    if (starting_animation()) {
        M_ending_color = color;
    }
    else {
        M_color = color;
    }
    return *this;
}

Object& Object::set_opacity(double opacity)
{
    auto color = M_color;
    opacity = std::clamp(opacity, 0.0, 1.0);
    color.a = static_cast<std::uint8_t>(opacity * 255.0);
    return set_color_with_alpha(color);
}

void Object::transformable_on_animate()
{
    M_starting_color = M_color;
    M_ending_color = M_color;
    object_on_animate();
}

void Object::transformable_on_animation_start()
{
    object_on_animation_start();
}

void Object::transformable_update_animation(double t)
{
    auto interp = [&](std::uint8_t v1, std::uint8_t v2) {
        auto diff = static_cast<int>(v2) - static_cast<int>(v1);
        return static_cast<std::uint8_t>(static_cast<int>(v1) + diff*t);
    };
    auto new_color = Color();
    new_color.r = interp(M_starting_color.r, M_ending_color.r);
    new_color.g = interp(M_starting_color.g, M_ending_color.g);
    new_color.b = interp(M_starting_color.b, M_ending_color.b);
    new_color.a = interp(M_starting_color.a, M_ending_color.a);
    set_color_with_alpha(new_color);
    object_update_animation(t);
}

void Object::transformable_on_animation_end()
{
    object_on_animation_end();
}
