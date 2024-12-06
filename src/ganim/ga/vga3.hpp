#ifndef GANIM_VGA3_HPP
#define GANIM_VGA3_HPP

/** @file
 * @brief Aliases and definitions for using 3D VGA
 */

#include "sga.hpp"
#include "print.hpp"

namespace ganim::vga3 {
    /** @brief The VGA metric.  You shouldn't need to use this. */
    constexpr auto metric = std::array<std::int8_t, 3>{1, 1, 1};
    /** @brief A general 2D VGA multivector.
     *
     * While you should be able to get away with using the other type aliases,
     * if you ever need to do something more funky, you can use this instead.
     * @tparam bases The basis multivectors present in this multivector.  See
     * @ref sga.hpp for more information.
     */
    template <std::uint64_t... bases>
    using Multivector = Multivector<double, metric, bases...>;
    using Scalar = Multivector<0>;
    using Vec = Multivector<1, 2, 4>;
    using Bivec = Multivector<3, 5, 6>;
    using Trivec = Multivector<7>;
    using Even = Multivector<0, 3, 5, 6>;
    constexpr auto e = Scalar(1);
    constexpr auto e1 = Vec(1, 0, 0);
    constexpr auto e2 = Vec(0, 1, 0);
    constexpr auto e3 = Vec(0, 0, 1);
    constexpr auto e12 = Bivec(1, 0, 0);
    constexpr auto e13 = Bivec(0, 1, 0);
    constexpr auto e23 = Bivec(0, 0, 1);
    constexpr auto e123 = Trivec(1);
    constexpr auto e21 = -e12;
    constexpr auto e31 = -e13;
    constexpr auto e32 = -e23;
    constexpr auto e231 = e123;
    constexpr auto e312 = e123;
    constexpr auto e213 = -e123;
    constexpr auto e132 = -e123;
    constexpr auto e321 = -e123;

    constexpr const char names[] = "123";

    template <std::uint64_t... bases>
    inline std::ostream& operator<<(
        std::ostream& stream,
        const ganim::vga3::Multivector<bases...>& m
    )
    {
        output_multivector<ganim::vga3::names>(stream, m);
        return stream;
    }
}

#endif
