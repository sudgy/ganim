#include "transformable.hpp"

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
{
    move_to(
        (p.blade_project<vga2::e1>()*e1 +
         p.blade_project<vga2::e2>()*e2 + e0).dual());
}

void Transformable::move_to(const vga3::Vector& p)
{
    move_to(
        (p.blade_project<vga3::e1>()*e1 +
         p.blade_project<vga3::e2>()*e2 +
         p.blade_project<vga3::e3>()*e3 + e0).dual());
}

void Transformable::move_to(const pga2::Bivector& p)
{
    const auto d = p.undual();
    move_to(
        (d.blade_project<pga2::e1>()*e1 +
         d.blade_project<pga2::e2>()*e2 +
         d.blade_project<pga2::e0>()*e0).dual());
}

void Transformable::move_to(const Trivector& p)
{
    auto current_center = (~M_rotor * e123 * M_rotor).grade_project<3>();
    auto new_center = p;
    current_center /= current_center.blade_project<e123>();
    new_center /= new_center.blade_project<e123>();
    apply_rotor(current_center * (current_center + new_center) / 2);
}
