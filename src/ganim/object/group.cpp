#include "group.hpp"

using namespace ganim;

void Group::add(Object& object)
{
    M_subobjects.push_back(&object);
}

Group& Group::apply_rotor(const pga3::Even& rotor)
{
    Transformable::apply_rotor(rotor);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->apply_rotor(rotor);
        }
    }
    return *this;
}

Group& Group::set_color(Color color)
{
    auto new_color = color;
    new_color.a = get_color().a;
    Object::set_color_with_alpha(new_color);
    for (auto obj : M_subobjects) {
        obj->set_color(color);
    }
    return *this;
}

Group& Group::set_color_with_alpha(Color color)
{
    Object::set_color_with_alpha(color);
    for (auto obj : M_subobjects) {
        obj->set_color_with_alpha(color);
    }
    return *this;
}

Group& Group::set_opacity(double opacity)
{
    Object::set_opacity(opacity);
    for (auto obj : M_subobjects) {
        obj->set_opacity(opacity);
    }
    return *this;
}

Group& Group::scale(const pga3::Trivector& about_point, double amount)
{
    M_propogate = false;
    Object::scale(about_point, amount);
    M_propogate = true;
    for (auto obj : M_subobjects) {
        obj->scale(about_point, amount);
    }
    return *this;
}

Group& Group::set_visible(bool visible)
{
    Object::set_visible(visible);
    for (auto obj : M_subobjects) {
        obj->set_visible(visible);
    }
    return *this;
}

void Group::set_draw_fraction(double value)
{
    Object::set_draw_fraction(value);
    const auto s = size();
    if (s == 0) return;
    const auto total_t = 1 + M_ratio * (s - 1);
    for (auto i = 0; i < s; ++i) {
        auto this_value = value * total_t - i * M_ratio;
        M_subobjects[i]->set_draw_fraction(std::clamp(this_value, 0.0, 1.0));
    }
}

void Group::set_draw_subobject_ratio(double ratio)
{
    if (ratio < 0 or ratio > 1) throw std::invalid_argument(
            "The draw subobject ratio must be between zero and 1.");
    M_ratio = ratio;
}
