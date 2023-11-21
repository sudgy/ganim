#include "object.hpp"

#include <iostream>

#include "ganim/ga/conversions.hpp"
#include "ganim/rate_functions.hpp"

using namespace ganim;
using namespace pga3;

Object& Object::set_visible(bool visible)
{
    M_visible = visible;
    on_set_visible(visible);
    return *this;
}

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
        on_color_changed(color);
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

Object& Object::fade_in(double time)
{
    set_visible(true);
    auto opacity = M_color.a / 255.0;
    set_opacity(0);
    animate(time, rf::linear);
    set_opacity(opacity);
    return *this;
}

Object& Object::fade_out(double time)
{
    M_real_opacity = M_color.a / 255.0;
    animate(time, rf::linear);
    set_opacity(0);
    return *this;
}

Object& Object::scale(double amount)
{
    return scale((~get_rotor()*e123*get_rotor()).grade_project<3>(), amount);
}

Object& Object::scale(const vga2::Vector& about_point, double amount)
{
    return scale(vga2_to_pga3(about_point), amount);
}

Object& Object::scale(const vga3::Vector& about_point, double amount)
{
    return scale(vga3_to_pga3(about_point), amount);
}

Object& Object::scale(const pga2::Vector& about_point, double amount)
{
    return scale(pga2_to_vga2_cheat(about_point), amount);
}

Object& Object::scale(const pga3::Vector& about_point, double amount)
{
    return scale(pga3_to_vga3_cheat(about_point), amount);
}

Object& Object::scale(const pga2::Bivector& about_point, double amount)
{
    return scale(pga2_to_pga3_flat(about_point), amount);
}

Object& Object::scale(const pga3::Trivector& about_point, double amount)
{
    auto new_about_point = about_point;
    new_about_point /= new_about_point.blade_project<e123>();
    new_about_point -= new_about_point.blade_project<e123>()*e123;
    auto center = (~get_rotor()*e123*get_rotor()).grade_project<3>();
    center -= new_about_point;
    auto new_center = center;
    new_center -= new_center.blade_project<e123>()*e123;
    new_center *= amount;
    new_center += e123;
    shift(new_center - center + e123);
    if (starting_animation()) {
        M_scale_point = about_point;
        M_ending_scale *= amount;
    }
    else {
        M_scale *= amount;
        on_scale(about_point, amount);
    }
    return *this;
}

double Object::get_scale() const
{
    return M_scale;
}

void Object::transformable_on_animate()
{
    M_starting_color = M_color;
    M_ending_color = M_color;
    M_starting_scale = M_scale;
    M_ending_scale = M_scale;
    M_scale_point = e123;
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
    auto current_scale
        = M_starting_scale + (M_ending_scale - M_starting_scale) * t;
    auto for_on_scale = current_scale / M_scale;
    M_scale = current_scale;
    on_scale(M_scale_point, for_on_scale);
    object_update_animation(t);
}

void Object::transformable_on_animation_end()
{
    if (M_real_opacity != -1) {
        set_opacity(M_real_opacity);
        M_real_opacity = -1;
        set_visible(false);
    }
    object_on_animation_end();
}
