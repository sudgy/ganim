#ifndef GANIM_UTIL_TRIANGLE_ROTOR_HPP
#define GANIM_UTIL_TRIANGLE_ROTOR_HPP

#include "ganim/ga/pga3.hpp"

namespace ganim {
    pga3::Even triangle_rotor(
        pga3::Trivector a1,
        pga3::Trivector a2,
        pga3::Trivector a3,
        pga3::Trivector b1,
        pga3::Trivector b2,
        pga3::Trivector b3
    );
}

#endif
