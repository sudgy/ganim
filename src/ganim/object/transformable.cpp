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
    if (starting_animation()) {
        M_new_rotor *= rotor;
    }
    else {
        M_rotor *= rotor;
        // Normalize the rotor
        auto norm2 = M_rotor * ~M_rotor;
        auto a = norm2.blade_project<e>();
        auto b = norm2.blade_project<e0123>();
        auto x = 1 / std::sqrt(a);
        auto y = -b*x*x*x/2;
        M_rotor *= x + y*e0123;
    }
    return *this;
}

Transformable& Transformable::move_to(const vga2::Vector& p)
    {return move_to(vga2_to_pga3(p));}
Transformable& Transformable::move_to(const vga3::Vector& p)
    {return move_to(vga3_to_pga3(p));}
Transformable& Transformable::move_to(const pga2::Vector& p)
    {return move_to(pga2_to_vga2_cheat(p));}
Transformable& Transformable::move_to(const pga3::Vector& p)
    {return move_to(pga3_to_vga3_cheat(p));}
Transformable& Transformable::move_to(const pga2::Bivector& p)
    {return move_to(pga2_to_pga3_flat(p));}

Transformable& Transformable::move_to(const Trivector& p)
{
    auto current_center = (~M_rotor * e123 * M_rotor).grade_project<3>();
    auto new_center = p;
    current_center /= current_center.blade_project<e123>();
    new_center /= new_center.blade_project<e123>();
    return apply_rotor(current_center * (current_center + new_center) / 2);
}

Transformable& Transformable::shift(const vga2::Vector& p)
    {return shift(vga2_to_pga3(p));}
Transformable& Transformable::shift(const vga3::Vector& p)
    {return shift(vga3_to_pga3(p));}
Transformable& Transformable::shift(const pga2::Vector& p)
    {return shift(pga2_to_vga2_cheat(p));}
Transformable& Transformable::shift(const pga3::Vector& p)
    {return shift(pga3_to_vga3_cheat(p));}
Transformable& Transformable::shift(const pga2::Bivector& p)
    {return shift(pga2_to_pga3_flat(p));}

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
    Transformable& start,
    Transformable& end,
    double t
)
{
    const auto& r1 = start.M_rotor;
    const auto& r2 = end.M_rotor;
    auto final_rotor = r1 * ga_exp(t*ga_log(~r1*r2));
    apply_rotor(~M_rotor * final_rotor);
}

void Transformable::on_animate()
{
    M_new_rotor = 1;
    transformable_on_animate();
}

void Transformable::on_animation_start()
{
    M_new_log = ga_log(M_new_rotor);
    M_new_rotor = M_rotor;
    transformable_on_animation_start();
}

void Transformable::update_animation(double t)
{
    auto new_rotor = M_new_rotor * ga_exp(M_new_log * t);
    apply_rotor(~M_rotor * new_rotor);
    transformable_update_animation(t);
}

void Transformable::on_animation_end()
{
    transformable_on_animation_end();
}
