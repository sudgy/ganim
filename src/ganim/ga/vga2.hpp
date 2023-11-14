#ifndef GANIM_VGA2_H
#define GANIM_VGA2_H

/** @file
 * @brief Aliases and definitions for using 2D VGA
 */

#include "sga.hpp"
#include "print.hpp"

namespace ganim::vga2 {
    /** @brief The VGA metric.  You shouldn't need to use this. */
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
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
    using Vector = Multivector<1, 2>;
    using Bivector = Multivector<3>;
    constexpr auto e = Scalar(1);
    constexpr auto e1 = Vector(1, 0);
    constexpr auto e2 = Vector(0, 1);
    constexpr auto e12 = Bivector(1);
    constexpr auto e21 = -e12;

    constexpr const char names[] = "12";
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
