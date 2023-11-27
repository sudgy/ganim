#include "group.hpp"

using namespace ganim;

void Group::add(Object& object)
{
    M_subobjects.push_back(&object);
}

Group& Group::apply_rotor(const pga3::Even& rotor)
{
    Transformable::apply_rotor(rotor);
    for (auto obj : M_subobjects) {
        obj->apply_rotor(rotor);
    }
    return *this;
}
