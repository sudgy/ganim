#ifndef GANIM_PGA3_H
#define GANIM_PGA3_H

/** @file
 * @brief Aliases and definitions for using 3D PGA
 */

#include "sga.hpp"
#include "print.hpp"

namespace ganim::pga3 {
    /** @brief The PGA metric.  You shouldn't need to use this. */
    constexpr auto metric = std::array<std::int8_t, 4>{0, 1, 1, 1};
    /** @brief A general 3D PGA multivector.
     *
     * While you should be able to get away with using the other type aliases,
     * if you ever need to do something more funky, you can use this instead.
     * @tparam bases The basis multivectors present in this multivector.  See
     * @ref sga.hpp for more information.
     */
    template <std::uint64_t... bases>
    using Multivector = Multivector<double, metric, bases...>;
    using Scalar = Multivector<0>;
    using Vector = Multivector<1, 2, 4, 8>;
    using Bivector = Multivector<3, 5, 6, 9, 10, 12>;
    using Trivector = Multivector<7, 11, 13, 14>;
    using Quadvector = Multivector<15>;
    constexpr auto e = Scalar(1);
    constexpr auto e0 = Vector(1, 0, 0, 0);
    constexpr auto e1 = Vector(0, 1, 0, 0);
    constexpr auto e2 = Vector(0, 0, 1, 0);
    constexpr auto e3 = Vector(0, 0, 0, 1);
    constexpr auto e01 = Bivector(1, 0, 0, 0, 0, 0);
    constexpr auto e02 = Bivector(0, 1, 0, 0, 0, 0);
    constexpr auto e12 = Bivector(0, 0, 1, 0, 0, 0);
    constexpr auto e03 = Bivector(0, 0, 0, 1, 0, 0);
    constexpr auto e13 = Bivector(0, 0, 0, 0, 1, 0);
    constexpr auto e23 = Bivector(0, 0, 0, 0, 0, 1);
    constexpr auto e012 = Trivector(1, 0, 0, 0);
    constexpr auto e013 = Trivector(0, 1, 0, 0);
    constexpr auto e023 = Trivector(0, 0, 1, 0);
    constexpr auto e123 = Trivector(0, 0, 0, 1);
    constexpr auto e0123 = Quadvector(1);
    constexpr auto e10 = -e01;
    constexpr auto e20 = -e02;
    constexpr auto e21 = -e12;
    constexpr auto e30 = -e03;
    constexpr auto e31 = -e13;
    constexpr auto e32 = -e23;
    constexpr auto e120 = e012;
    constexpr auto e201 = e012;
    constexpr auto e102 = -e012;
    constexpr auto e021 = -e012;
    constexpr auto e210 = -e012;
    constexpr auto e130 = e013;
    constexpr auto e301 = e013;
    constexpr auto e103 = -e013;
    constexpr auto e031 = -e013;
    constexpr auto e310 = -e013;
    constexpr auto e230 = e023;
    constexpr auto e302 = e023;
    constexpr auto e203 = -e023;
    constexpr auto e032 = -e023;
    constexpr auto e320 = -e023;
    constexpr auto e231 = e123;
    constexpr auto e312 = e123;
    constexpr auto e213 = -e123;
    constexpr auto e132 = -e123;
    constexpr auto e321 = -e123;
    constexpr auto e0132 = -e0123;
    constexpr auto e0213 = -e0123;
    constexpr auto e0231 = e0123;
    constexpr auto e0312 = e0123;
    constexpr auto e0321 = -e0123;
    constexpr auto e1023 = -e0123;
    constexpr auto e1032 = e0123;
    constexpr auto e1203 = e0123;
    constexpr auto e1230 = -e0123;
    constexpr auto e1302 = -e0123;
    constexpr auto e1320 = e0123;
    constexpr auto e2013 = e0123;
    constexpr auto e2031 = -e0123;
    constexpr auto e2103 = -e0123;
    constexpr auto e2130 = e0123;
    constexpr auto e2301 = e0123;
    constexpr auto e2310 = -e0123;
    constexpr auto e3012 = -e0123;
    constexpr auto e3021 = e0123;
    constexpr auto e3102 = e0123;
    constexpr auto e3120 = -e0123;
    constexpr auto e3201 = -e0123;
    constexpr auto e3210 = e0123;

    constexpr const char names[] = "0123";
    template <std::uint64_t... bases>
    std::ostream& operator<<(
        std::ostream& stream,
        const Multivector<bases...>& m
    )
    {
        output_multivector<names>(stream, m);
        return stream;
    }
}

#endif
