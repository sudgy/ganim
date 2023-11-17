#include "transformable.hpp"

#include "ganim/ga/conversions.hpp"
#include "ganim/ga/exp.hpp"

using namespace ganim;
using namespace pga3;

Transformable::Transformable() {}

Transformable::Transformable(const Even& rotor) : M_rotor(rotor) {}

void Transformable::reset()
{
    apply_rotor(~M_rotor);
}

void Transformable::apply_rotor(const Even& rotor)
{
    M_rotor *= rotor;
    on_apply_rotor(rotor);
}

void Transformable::move_to(const vga2::Vector& p)
    {move_to(vga2_to_pga3(p));}
void Transformable::move_to(const vga3::Vector& p)
    {move_to(vga3_to_pga3(p));}
void Transformable::move_to(const pga2::Vector& p)
    {move_to(pga2_to_vga2_cheat(p));}
void Transformable::move_to(const pga3::Vector& p)
    {move_to(pga3_to_vga3_cheat(p));}
void Transformable::move_to(const pga2::Bivector& p)
    {move_to(pga2_to_pga3_flat(p));}

void Transformable::move_to(const Trivector& p)
{
    auto current_center = (~M_rotor * e123 * M_rotor).grade_project<3>();
    auto new_center = p;
    current_center /= current_center.blade_project<e123>();
    new_center /= new_center.blade_project<e123>();
    apply_rotor(current_center * (current_center + new_center) / 2);
}

void Transformable::shift(const vga2::Vector& p)
    {shift(vga2_to_pga3(p));}
void Transformable::shift(const vga3::Vector& p)
    {shift(vga3_to_pga3(p));}
void Transformable::shift(const pga2::Vector& p)
    {shift(pga2_to_vga2_cheat(p));}
void Transformable::shift(const pga3::Vector& p)
    {shift(pga3_to_vga3_cheat(p));}
void Transformable::shift(const pga2::Bivector& p)
    {shift(pga2_to_pga3_flat(p));}

void Transformable::shift(const Trivector& p)
{
    auto new_p = p;
    new_p /= p.blade_project<e123>();
    apply_rotor(e123 * (e123 + new_p) / 2);
}

void Transformable::rotate(double angle)
    {rotate(e12, angle);}
void Transformable::rotate(const vga2::Vector& about_point, double angle)
    {rotate(vga2_to_pga2(about_point), angle);}
void Transformable::rotate(const pga2::Vector& about_point, double angle)
    {rotate(pga2_to_vga2_cheat(about_point), angle);}
void Transformable::rotate(const vga3::Bivector& about_plane, double angle)
{
    apply_rotor(vga3_to_pga3(ga_exp(about_plane * angle / 2)));
}
void Transformable::rotate(const pga2::Bivector& about_point, double angle)
    {rotate(pga2_to_pga3(about_point), angle);}
void Transformable::rotate(const pga3::Bivector& about_line, double angle)
{
    apply_rotor(ga_exp(about_line * angle / 2));
}

void Transformable::on_animation_start()
{

}

void Transformable::update_animation(double t)
{

}

void Transformable::on_animation_end()
{

}
