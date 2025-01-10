#include "animatable.hpp"

using namespace ganim;

void Animatable::set_fps(int fps)
{
    M_fps = fps;
}

ObjectPtr<Animatable> Animatable::polymorphic_copy() const
{
    return ObjectPtr<Animatable>::from_new(polymorphic_copy_impl());
}

Animatable* Animatable::polymorphic_copy_impl() const
{
    return new Animatable(*this);
}

void Animatable::interpolate(const Animatable&, const Animatable&, double) {}
