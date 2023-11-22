#include "group.hpp"

using namespace ganim;

void Group::add(Object& object)
{
    M_subobjects.push_back(&object);
}
