#include "group.hpp"

using namespace ganim;

void Group::draw(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) drawable->draw(camera);
    }
}

bool Group::is_visible() const
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) return true;
    }
    return false;
}

void Group::draw_outline(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) drawable->draw_outline(camera);
    }
}

void Group::set_outline(const Color& color, double thickness)
{
    M_outline_color = color;
    M_outline_thickness = thickness;
    for (auto drawable : M_subobjects) {
        drawable->set_outline(color, thickness);
    }
}

void Group::invalidate_outline()
{
    for (auto drawable : M_subobjects) {
        drawable->invalidate_outline();
    }
}

Color Group::get_outline_color() const
{
    return M_outline_color;
}

double Group::get_outline_thickness() const
{
    return M_outline_thickness;
}
