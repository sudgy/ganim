#ifndef GANIM_GA_EXP_H
#define GANIM_GA_EXP_H

/** @file
 * @brief Contains everything related to exponentiating in geometric algebra
 *
 * It also brought a few other things along for the ride, such as inverses and
 * logarithms.  Note that because I currently don't care about being as general
 * as possible, I only define all of these operations for simple multivectors
 * and for the interesting cases in 3D PGA.  Everything else will fail.
 */

#include "sga.hpp"
#include "pga3.hpp"

namespace ganim {

/** @brief A type trait saying whether a multivector is always simple.
 *
 * This is the default definition, which is always false.
 */
template <typename T>
inline constexpr bool always_simple = false;

/** @brief A type trait saying whether a multivector is always simple.
 *
 * In the end, this is just checking if the type is a multivector that is either
 * a scalar, a vector, a bivector, or a trivector.
 */
template <
    scalar Scalar,
    auto metric,
    std::uint64_t... bases
> requires(((bases == 0) and ...) or
           ((std::popcount(bases) == 1) and ...) or
           ((std::popcount(bases) == metric.size() - 1) and ...) or
           ((std::popcount(bases) == metric.size()) and ...))
inline constexpr bool always_simple<Multivector<Scalar, metric, bases...>>
    = true;

/** @brief Calculates the inverse of a multivector
 *
 * Note that this can't calculate most inverses.  It can only calculate the
 * inverse of either a simple multivector or a 3D PGA bivector.  I don't care
 * about inverses of other multivectors at the moment so I haven't bothered with
 * them.  A `static_assert` will fire if you try to pass any other type of
 * multivector.
 *
 * @tparam T The type of multivector you are passing in.  Assumed to be a @ref
 * ganim::Multivector "Multivector".
 * @param v The multivector to invert.  If it is not simple or a 3D PGA
 * bivector, the program is ill-formed.
 */
template <typename T>
constexpr auto ga_inv(const T& v)
{
    if constexpr (always_simple<T>) return ~v / v.norm2();
    else {
        static_assert(std::is_same_v<T, pga3::Bivector>,
            "ga_inv is currently only implemented for provably-simple "
            "multivectors and for 3D PGA bivectors.  If you know your "
            "multivector is simple, just use ~v / v.norm2().  If you don't, "
            "then you're out of luck."
        );
        using namespace pga3;
        const Bivector& b = v;
        const auto r = ~b;
        const auto s = b*r;
        const auto c = s.blade_project<e>()
                     - s.blade_project<e0123>()*e0123;
        return r * (c / (s * c).blade_project<e>());
    }
}

/** @brief Calculates the exponential of a simple multivector
 *
 * It's important to note that this function only works for simple multivectors!
 * If the multivector is not simple, the result is unspecified.  The only
 * guarantee in this case is that if the input is close to a simple multivector,
 * the output will be close to the exponential of that simple multivector.
 *
 * @tparam T The type of multivector you are passing in.  Assumed to be a @ref
 * ganim::Multivector "Multivector".
 * @param v The simple multivector to exponentiate.  If it is not simple, the
 * result is mostly unspecified.
 */
template <typename T>
constexpr auto simple_exp(const T& v)
{
    auto v2 = (v*v).template binary_blade_project<0>();
    if (v2 == 0) return 1 + v;
    auto v_mag = std::sqrt(std::abs(v2));
    auto v_unit = v / v_mag;
    if (v2 < 0) return std::cos(v_mag) + std::sin(v_mag)*v_unit;
    return std::cosh(v_mag) + std::sinh(v_mag)*v_unit;
}

/** @brief Calculates the exponential of a multivector
 *
 * Note that this can't calculate most exponentials.  It can only calculate the
 * exponential of either a simple multivector or a 3D PGA bivector.  I don't
 * care about exponentials of other multivectors at the moment so I haven't
 * bothered with them.  A `static_assert` will fire if you try to pass any other
 * type of multivector.
 *
 * @tparam T The type of multivector you are passing in.  Assumed to be a @ref
 * ganim::Multivector "Multivector".
 * @param v The multivector to exponentiate.  If it is not simple or a 3D PGA
 * bivector, the program is ill-formed.
 */
template <typename T>
constexpr auto ga_exp(const T& v)
{
    if constexpr (always_simple<T>) return simple_exp(v);
    {
        static_assert(std::is_same_v<T, pga3::Bivector>,
            "ga_exp is currently only implemented for provably-simple "
            "multivectors and for 3D PGA bivectors.  If you know your "
            "multivector is simple, use simple_exp.  If you don't, then you're "
            "out of luck."
        );
        using namespace pga3;
        const Bivector& b = v;
        if ((b*b).blade_project<e>() == 0) return 1 + v + 0*e0123;
        const auto b1 = (b ^ b) * ga_inv(b) / 2;
        const auto b2 = b - b1;
        return simple_exp(b1) * simple_exp(b2);
    }
}

}

#endif
