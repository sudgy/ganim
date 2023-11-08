#ifndef GANIM_VGA2_H
#define GANIM_VGA2_H

#include "sga.hpp"

namespace ganim::vga2 {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    template <std::uint64_t... bases>
    using Multivector = Multivector<double, metric, bases...>;
    using Scalar = Multivector<0>;
    using Vector = Multivector<1, 2>;
    using Bivector = Multivector<3>;
    constexpr auto e = Scalar(1);
    constexpr auto e1 = Vector(1, 0);
    constexpr auto e2 = Vector(0, 1);
    constexpr auto e12 = Bivector(1);
    constexpr auto e21 = -e12;
}

#endif
