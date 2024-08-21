#include "cluster.hpp"

using namespace ganim;

void Cluster::draw(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        drawable->draw_outline(camera);
        drawable->draw(camera);
    }
}

bool Cluster::is_visible() const
{
    return Object::is_visible();
}

void Cluster::draw_outline(const Camera&)
{
}

void Cluster::set_outline(const Color& color, double thickness)
{
    for (auto drawable : M_subobjects) {
        drawable->set_outline(color, thickness);
    }
}

void Cluster::invalidate_outline()
{
    for (auto drawable : M_subobjects) {
        drawable->invalidate_outline();
    }
}
