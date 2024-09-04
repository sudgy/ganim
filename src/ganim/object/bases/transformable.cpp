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

Trivector Transformable::get_origin() const
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

Transformable& Transformable::move_to(const Trivector& p)
{
    auto current_center = (~M_rotor * e123 * M_rotor).grade_project<3>();
    auto new_center = p;
    current_center /= current_center.blade_project<e123>();
    new_center /= new_center.blade_project<e123>();
    return apply_rotor(current_center * (current_center + new_center) / 2);
}

Transformable& Transformable::shift(const Trivector& p)
{
    auto new_p = p;
    new_p /= p.blade_project<e123>();
    return apply_rotor(-e123 * (e123 + new_p) / 2);
}

Transformable& Transformable::rotate(double angle)
    {return rotate(e12, angle);}
Transformable& Transformable::rotate(
    const vga2::Vector& about_point,
    double angle
)
    {return rotate(vga2_to_pga2(about_point), angle);}
Transformable& Transformable::rotate(
    const pga2::Vector& about_point,
    double angle
)
    {return rotate(pga2_to_vga2_cheat(about_point), angle);}
Transformable& Transformable::rotate(
    const vga3::Bivector& about_plane,
    double angle
)
{
    return apply_rotor(vga3_to_pga3(ga_exp(about_plane * angle / 2)));
}
Transformable& Transformable::rotate(
    const pga2::Bivector& about_point,
    double angle
)
    {return rotate(pga2_to_pga3(about_point), angle);}
Transformable& Transformable::rotate(
    const pga3::Bivector& about_line,
    double angle
)
{
    return apply_rotor(ga_exp(about_line * angle / 2));
}

void Transformable::interpolate(
    const Transformable& start,
    const Transformable& end,
    double t
)
{
    const auto& r1 = start.M_rotor;
    const auto& r2 = end.M_rotor;
    auto final_rotor = r1 * ga_exp(t*ga_log(~r1*r2));
    apply_rotor(~M_rotor * final_rotor);
}
