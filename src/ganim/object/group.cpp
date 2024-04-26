#include "group.hpp"

#include <format>

using namespace ganim;

void Group::add(Object& object)
{
    M_subobjects.push_back(&object);
}

std::unique_ptr<Group> Group::anim_copy() const
{
    struct OwningGroup : public Group {
        OwningGroup(const Group& other) : Group(other) {}
        std::vector<std::unique_ptr<Object>> M_owned_subobjects;
    };
    auto result = std::make_unique<OwningGroup>(*this);
    result->M_subobjects.clear();
    for (auto obj : M_subobjects) {
        if (auto subgroup = obj->as_group()) {
            result->M_owned_subobjects.emplace_back(subgroup->anim_copy());
        }
        else {
            result->M_owned_subobjects.emplace_back(obj->anim_copy());
        }
        result->M_subobjects.push_back(result->M_owned_subobjects.back().get());
    }
    return result;
}

void Group::interpolate(const Group& start, const Group& end, double t)
{
    if (size() != start.size()) {
        throw std::invalid_argument(std::format(
            "When interpolating a group of size {}, the size of the starting "
            "group is {}, when the two sizes should be equal.",
            size(),
            start.size()
        ));
    }
    if (size() != end.size()) {
        throw std::invalid_argument(std::format(
            "When interpolating a group of size {}, the size of the ending "
            "group is {}, when the two sizes should be equal.",
            size(),
            end.size()
        ));
    }
    for (auto i = 0; i < size(); ++i) {
        auto group1 = (*this)[i].as_group();
        auto group2 = start[i].as_group();
        auto group3 = end[i].as_group();
        if (group1) {
            if (!group2) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is a "
                    "subgroup, while it is not a subgroup in the starting "
                    "group.",
                    i
                ));
            }
            if (!group3) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is a "
                    "subgroup, while it is not a subgroup in the ending "
                    "group.",
                    i
                ));
            }
        }
        else {
            if (group2) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is not "
                    "a subgroup, while it is a subgroup in the starting group.",
                    i
                ));
            }
            if (group3) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is not "
                    "a subgroup, while it is not a subgroup in the ending ",
                    "group.",
                    i
                ));
            }
        }
    }
    M_propogate = false;
    Object::interpolate(start, end, t);
    M_propogate = true;
    for (auto i = 0; i < size(); ++i) {
        auto& obj = (*this)[i];
        if (auto group = obj.as_group()) {
            group->interpolate(*start[i].as_group(), *end[i].as_group(), t);
        }
        else {
            obj.interpolate(start[i], end[i], t);
        }
    }
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
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_color(color);
        }
    }
    return *this;
}

Group& Group::set_color_with_alpha(Color color)
{
    Object::set_color_with_alpha(color);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_color_with_alpha(color);
        }
    }
    return *this;
}

Group& Group::set_opacity(double opacity)
{
    Object::set_opacity(opacity);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_opacity(opacity);
        }
    }
    return *this;
}

Group& Group::scale(const pga3::Trivector& about_point, double amount)
{
    if (M_propogate) {
        M_propogate = false;
        Object::scale(about_point, amount);
        M_propogate = true;
        for (auto obj : M_subobjects) {
            obj->scale(about_point, amount);
        }
    }
    else Object::scale(about_point, amount);
    return *this;
}

Group& Group::set_visible(bool visible)
{
    Object::set_visible(visible);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_visible(visible);
        }
    }
    return *this;
}

void Group::set_draw_fraction(double value)
{
    Object::set_draw_fraction(value);
    if (M_propogate) {
        const auto s = size();
        if (s == 0) return;
        const auto total_t = 1 + M_ratio * (s - 1);
        for (auto i = 0; i < s; ++i) {
            auto this_value = value * total_t - i * M_ratio;
            M_subobjects[i]->set_draw_fraction(std::clamp(this_value, 0.0, 1.0));
        }
    }
}

void Group::set_creating(bool creating)
{
    Object::set_creating(creating);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_creating(creating);
        }
    }
}

void Group::set_noise_creating(double noise_creating)
{
    Object::set_noise_creating(noise_creating);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_noise_creating(noise_creating);
        }
    }
}

void Group::set_draw_subobject_ratio(double ratio)
{
    if (ratio < 0 or ratio > 1) throw std::invalid_argument(
            "The draw subobject ratio must be between zero and 1.");
    M_ratio = ratio;
}
