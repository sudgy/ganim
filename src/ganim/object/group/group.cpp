#include "group.hpp"

using namespace ganim;

void Group::add(Object& object)
{
    GroupBase::add(object);
    if (auto drawable = dynamic_cast<Drawable*>(&object)) {
        M_drawables.push_back(drawable);
    }
}

void Group::draw(const Camera& camera)
{
    for (auto drawable : M_drawables) {
        drawable->draw(camera);
    }
}

bool Group::is_visible() const
{
    return Object::is_visible();
}