#ifndef GANIM_PGA2_HPP
#define GANIM_PGA2_HPP

/** @file
 * @brief Aliases and definitions for using 2D PGA
 */

#include "sga.hpp"
#include "print.hpp"

namespace ganim::pga2 {
    /** @brief The PGA metric.  You shouldn't need to use this. */
    constexpr auto metric = std::array<std::int8_t, 3>{0, 1, 1};
    /** @brief A general 2D PGA multivector.
     *
     * While you should be able to get away with using the other type aliases,
     * if you ever need to do something more funky, you can use this instead.
     * @tparam bases The basis multivectors present in this multivector.  See
     * @ref sga.hpp for more information.
     */
    template <std::uint64_t... bases>
    using Multivector = Multivector<double, metric, bases...>;
    using Scalar = Multivector<0>;
    using Vector = Multivector<1, 2, 4>;
    using Bivector = Multivector<3, 5, 6>;
    using Trivector = Multivector<7>;
    using Even = Multivector<0, 3, 5, 6>;
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

    constexpr const char names[] = "012";

    template <std::uint64_t... bases>
    inline std::ostream& operator<<(
        std::ostream& stream,
        const ganim::pga2::Multivector<bases...>& m
    )
    {
        output_multivector<ganim::pga2::names>(stream, m);
        return stream;
    }
}

#endif
