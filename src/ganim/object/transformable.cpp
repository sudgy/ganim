#include "transformable.hpp"

using namespace ganim;

Transformable::Transformable() {}

Transformable::Transformable(const pga3::Even& rotor) : M_rotor(rotor) {}

void Transformable::reset()
{
    apply_rotor(~M_rotor);
}

void Transformable::apply_rotor(const pga3::Even& rotor)
{
    M_rotor *= rotor;
    on_apply_rotor(rotor);
}
