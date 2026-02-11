#include "animatable.hpp"

using namespace ganim;

void Animatable::set_fps(int fps)
{
    M_fps = fps;
}

ObjectPtr<Animatable> Animatable::copy() const
{
    return ObjectPtr<Animatable>::from_new(copy_impl());
}

Animatable* Animatable::copy_impl() const
{
    return new Animatable(*this);
}

void Animatable::interpolate(const Animatable&, const Animatable&, double) {}
