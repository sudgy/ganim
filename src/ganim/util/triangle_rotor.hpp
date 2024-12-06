#ifndef GANIM_UTIL_TRIANGLE_ROTOR_HPP
#define GANIM_UTIL_TRIANGLE_ROTOR_HPP

#include "ganim/ga/pga3.hpp"

namespace ganim {
    pga3::Even triangle_rotor(
        pga3::Trivec a1,
        pga3::Trivec a2,
        pga3::Trivec a3,
        pga3::Trivec b1,
        pga3::Trivec b2,
        pga3::Trivec b3
    );
}

#endif
