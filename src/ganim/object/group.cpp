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
