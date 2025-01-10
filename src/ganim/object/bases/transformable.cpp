#include "transformable.hpp"

#include "ganim/ga/conversions.hpp"
#include "ganim/ga/exp.hpp"

using namespace ganim;
using namespace pga3;

Transformable::Transformable() {}

Transformable::Transformable(const Even& rotor) : M_rotor(rotor) {}

Transformable& Transformable::reset()
{
    apply_rotor(~M_rotor);
    return *this;
}

Trivec Transformable::get_origin() const
{
    return (~M_rotor * e123 * M_rotor).grade_project<3>();
}

Transformable& Transformable::apply_rotor(const vga2::Even& rotor)
{
    return apply_rotor(vga2_to_pga3(rotor));
}

Transformable& Transformable::apply_rotor(const vga3::Even& rotor)
{
    return apply_rotor(vga3_to_pga3(rotor));
}

Transformable& Transformable::apply_rotor(const pga2::Even& rotor)
{
    return apply_rotor(pga2_to_pga3(rotor));
}

Transformable& Transformable::apply_rotor(const Even& rotor)
{
    M_rotor *= rotor;
    // Normalize the rotor
    auto norm2 = M_rotor * ~M_rotor;
    auto a = norm2.blade_project<e>();
    auto b = norm2.blade_project<e0123>();
    auto x = 1 / std::sqrt(a);
    auto y = -b*x*x*x/2;
    M_rotor *= x + y*e0123;
    return *this;
}

Transformable& Transformable::move_to(const Trivec& p)
{
    auto current_center = (~M_rotor * e123 * M_rotor).grade_project<3>();
    auto new_center = p;
    current_center /= current_center.blade_project<e123>();
    new_center /= new_center.blade_project<e123>();
    return apply_rotor(-current_center * (current_center + new_center) / 2);
}

Transformable& Transformable::shift(const Trivec& p)
{
    auto new_p = p;
    new_p /= p.blade_project<e123>();
    return apply_rotor(-e123 * (e123 + new_p) / 2);
}

Transformable& Transformable::rotate(double angle)
    {return rotate(angle, e12);}
Transformable& Transformable::rotate(
    double angle,
    const vga2::Vec& about_point
)
    {return rotate(angle, vga2_to_pga2(about_point));}
Transformable& Transformable::rotate(
    double angle,
    const pga2::Vec& about_point
)
    {return rotate(angle, pga2_to_vga2_cheat(about_point));}
Transformable& Transformable::rotate(
    double angle,
    const vga3::Bivec& about_plane
)
{
    return apply_rotor(vga3_to_pga3(ga_exp(about_plane * angle / 2)));
}
Transformable& Transformable::rotate(
    double angle,
    const pga2::Bivec& about_point
)
{
    return rotate(angle, pga2_to_pga3(about_point));
}
Transformable& Transformable::rotate(const pga2::Bivec& about_point)
{
    return rotate(1, about_point);
}
Transformable& Transformable::rotate(
    double angle,
    const pga3::Bivec& about_line
)
{
    return apply_rotor(ga_exp(about_line * angle / 2));
}
Transformable& Transformable::rotate(
    const pga3::Bivec& about_line
)
{
    return rotate(1, about_line);
}

void Transformable::interpolate(
    const Animatable& start,
    const Animatable& end,
    double t
)
{
    auto start2 = dynamic_cast<const Transformable*>(&start);
    auto end2 = dynamic_cast<const Transformable*>(&end);
    const auto& r1 = start2->M_rotor;
    const auto& r2 = end2->M_rotor;
    auto final_rotor = r1 * ga_exp(t*ga_log(~r1*r2));
    apply_rotor(~M_rotor * final_rotor);
}

ObjectPtr<Transformable> Transformable::polymorphic_copy() const
{
    return ObjectPtr<Transformable>::from_new(polymorphic_copy_impl());
}

Transformable* Transformable::polymorphic_copy_impl() const
{
    return new Transformable(*this);
}
