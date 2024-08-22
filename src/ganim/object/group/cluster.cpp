#include "cluster.hpp"

using namespace ganim;

void Cluster::draw(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) {
            drawable->draw_outline(camera);
            drawable->draw(camera);
        }
    }
}

bool Cluster::is_visible() const
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) return true;
    }
    return false;
}

void Cluster::draw_outline(const Camera&)
{
}

void Cluster::set_outline(const Color& color, double thickness)
{
    M_outline_color = color;
    M_outline_thickness = thickness;
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

Color Cluster::get_outline_color() const
{
    return M_outline_color;
}

double Cluster::get_outline_thickness() const
{
    return M_outline_thickness;
}
