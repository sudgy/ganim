#ifndef GANIM_UTIL_TRIANGLE_ROTOR_HPP
#define GANIM_UTIL_TRIANGLE_ROTOR_HPP

#include "ganim/ga/pga3.hpp"

namespace ganim {
    /** @brief Finds the rotor that will bring the triangle at points a1, a2,
     * and a3 to the triangle at points b1, b2, and b3.  I thought I would use
     * this somewhere and ended up not using it so it's just sitting here doing
     * nothing currently.  If there isn't any rotor that roughly brings one of
     * the triangles into the other, the behavior is undefined.
     */
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
