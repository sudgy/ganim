#include "object.hpp"

using namespace ganim;

Object& Object::set_color(Color color)
{
    M_color.r = color.r;
    M_color.g = color.g;
    M_color.b = color.b;
    return *this;
}

Object& Object::set_color_with_alpha(Color color)
{
    M_color = color;
    return *this;
}

Object& Object::set_opacity(double opacity)
{
    opacity = std::clamp(opacity, 0.0, 1.0);
    M_color.a = static_cast<std::uint8_t>(opacity * 255.0);
    return *this;
}
