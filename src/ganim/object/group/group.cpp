#include "group.hpp"

using namespace ganim;

void Group::draw(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        drawable->draw(camera);
    }
}

bool Group::is_visible() const
{
    return Object::is_visible();
}

void Group::draw_outline(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        drawable->draw_outline(camera);
    }
}

void Group::set_outline(const Color& color, double thickness)
{
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
