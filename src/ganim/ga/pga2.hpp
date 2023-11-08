#ifndef GANIM_PGA2_H
#define GANIM_PGA2_H

#include "sga.hpp"

namespace ganim::pga2 {
    constexpr auto metric = std::array<std::int8_t, 3>{0, 1, 1};
    template <std::uint64_t... bases>
    using Multivector = Multivector<double, metric, bases...>;
    using Scalar = Multivector<0>;
    using Vector = Multivector<1, 2, 4>;
    using Bivector = Multivector<3, 5, 6>;
    using Trivector = Multivector<7>;
    constexpr auto e = Scalar(1);
    constexpr auto e0 = Vector(1, 0, 0);
    constexpr auto e1 = Vector(0, 1, 0);
    constexpr auto e2 = Vector(0, 0, 1);
    constexpr auto e01 = Bivector(1, 0, 0);
    constexpr auto e02 = Bivector(0, 1, 0);
    constexpr auto e12 = Bivector(0, 0, 1);
    constexpr auto e012 = Trivector(1);
    constexpr auto e10 = -e01;
    constexpr auto e20 = -e02;
    constexpr auto e21 = -e12;
    constexpr auto e120 = e012;
    constexpr auto e201 = e012;
    constexpr auto e102 = -e012;
    constexpr auto e021 = -e012;
    constexpr auto e210 = -e012;
}

#endif
