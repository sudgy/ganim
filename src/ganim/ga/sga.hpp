#ifndef GANIM_SUDGY_GEOMETRIC_ALGEBRA_HPP
#define GANIM_SUDGY_GEOMETRIC_ALGEBRA_HPP

/**
 * @file
 * @brief The basic geometric algebra definitions
 *
 * You will usually not need to use anything in this file directly.  Instead,
 * you usually will use the files @ref vga2.hpp, @ref vga3.hpp, @ref pga2.hpp,
 * or @ref pga3.hpp.  If you want to use another flavor, I would suggest making
 * something similar to these files.
 *
 * In this library, basis blades are represented using unsigned 64-bit integers.
 * These integers represent bitmasks of what basis vectors are included.  For
 * example, in a 4D algebra with basis vectors e1, e2, e3, and e4, the basis
 * vectors would represent the numbers 1, 2, 4, and 8.  Then other basis
 * multivectors would be some combination of these.  For example, 13 would
 * represent the basis multivector e134, because 13 = 1 + 4 + 8.  All basis
 * multivectors are assumed to be in lexicographic order.
 *
 * The metric which is often passed around as a template parameter is intended
 * to be an `std::array<std::int8_t, N>`, where `N` is the dimension, and each
 * entry of the array is 1, -1, or 0.
 */

#include <cstdint>
#include <cmath>
#include <array>
#include <vector>
#include <set>
#include <algorithm>
#include <concepts>
#include <stdexcept>

namespace ganim {

/** @brief The maximum dimension support by SGA
 *
 * Because SGA represents basis blades with bitmasks using 64-bit unsigned
 * integers, it simply just doesn't accept anything with a dimension above 64.
 * Thankfully, I've never heard of any application needing a dimension that
 * high.  Compile times already get pretty slow when you push the dimension
 * higher so honestly you probably can't even get a 64D algebra.
 */
constexpr unsigned int GC_max_dimension = 64;

/** @brief A concept specifying what can be used as a scalar in geometric
 * algebra
 */
template <typename T>
concept scalar = std::regular<T> and requires(const T& a, const T& b, T c) {
    {a + b} -> std::convertible_to<T>;
    {a - b} -> std::convertible_to<T>;
    {a * b} -> std::convertible_to<T>;
    {a / b} -> std::convertible_to<T>;
    {+b} -> std::convertible_to<T>;
    {-b} -> std::convertible_to<T>;
    {c = 0};
    {c = 1};
    {c = -1};
};

/** @brief Calculates what sign the product of two basis multivectors needs to
 * have
 * @return +1, -1, or 0, which is what the product of the scalar coefficients
 * should be multiplied by
 *
 * This function is intended for internal use only.
 * */
template <
    scalar Scalar,
    std::uint64_t blade1,
    std::uint64_t blade2,
    std::uint64_t N
>
consteval
Scalar basis_blade_product_sign(const std::array<std::int8_t, N>& metric)
{
    static_assert(N <= GC_max_dimension,
            "The metric you are trying to use has too high of a dimension.");
    auto sign = false;
    auto parity = false;
    for (std::uint8_t i = 0; i < N; ++i) {
        if (blade1 & (1UL << i)) parity = !parity;
    }
    for (std::uint8_t i = 0; i < N; ++i) {
        const auto first = static_cast<bool>(blade1 & (1UL << i));
        const auto second = static_cast<bool>(blade2 & (1UL << i));
        if (first) parity = !parity;
        if (second) sign ^= parity;
        if (first and second) {
            if (metric[i] == -1) sign = !sign;
            else if (metric[i] == 0) return 0;
        }
    }
    if (sign) return -1;
    else return 1;
}

/** @brief Calculates what sign the dual of a basis multivector should be
 * @return +1 or -1 depending on what should be multiplied by `~b` to produce
 * the dual
 *
 * This function is intended for internal use only.
 */
template <std::size_t N>
consteval int dual_sign(std::uint64_t b)
{
    auto sign = false;
    auto parity = false;
    for (std::uint8_t i = 0; i < N; ++i) {
        const auto bit = static_cast<bool>(b & (1UL << i));
        if (bit) sign ^= parity;
        else parity = !parity;
    }
    return sign ? -1 : 1;
}

/** @brief Represents a scalar multiple of a single basis blade
 *
 * This is basically just a wrapper around a scalar value, but typed in such a
 * way that it represents a basis blade.  Note that you can't actually do any GA
 * operations on these, and honestly you shouldn't use them.  You should always
 * just use a @ref Multivector instead.
 * @tparam Scalar a @ref ganim::scalar "scalar" type that represents the
 * coefficient of the basis blade
 * @tparam metric The metric being used.  See @ref sga.hpp for more details.
 * @tparam basis The basis blade represented as an unsigned integer.  See @ref
 * sga.hpp for more details.
 */
template <
    scalar Scalar,
    auto metric,
    std::uint64_t basis
>
struct BasisBlade {
    Scalar coefficient = 0;
    constexpr BasisBlade(Scalar x) : coefficient(x) {}
    constexpr BasisBlade()=default;
    constexpr bool operator==(const BasisBlade&) const=default;
};

/** @brief The main multivector class
 *
 * Note that the template arguments are kind of annoying to get right, so it's
 * suggested that you use typedefs like in @ref src/ganim/ga/vga2.hpp
 * "vga2.hpp", @ref src/ganim/ga/vga3.hpp "vga3.hpp", @ref src/ganim/ga/pga2.hpp
 * "pga2.hpp", or @ref src/ganim/ga/pga3.hpp "pga3.hpp".  Those files, along
 * with the description of the basis blades in @ref sga.hpp, should be enough to
 * help you if you find yourself needing your own specialization.
 *
 * Note that while this class derives from several @ref BasisBlade
 * "BasisBlade"s, that is an implementation detail, and you should never use it!
 * In fact, they were originally private bases of this class, but I changed it
 * to public later just to allow for multivectors to be used as non-type
 * template arguments.
 *
 * @tparam Scalar a @ref ganim::scalar "scalar" type that represents the
 * coefficients used in this multivector
 * @tparam metric The metric being used.  See @ref sga.hpp for more details.
 * @tparam bases All of the basis blades present in this multivector.  Note that
 * it's technically doable to have some components of one grade without others.
 * For example, using 1, 4 as your values for `bases` corresponds to having an
 * e1 component and an e3 component without an e2 component.  However, doing
 * this is not advised.
 */
template <
    scalar Scalar,
    auto metric,
    std::uint64_t... bases
> requires(std::ranges::is_sorted(
            std::array<std::uint64_t, sizeof...(bases)>{bases...}))
class Multivector : public BasisBlade<Scalar, metric, bases>... {
    template <scalar Scalar2, auto metric2, std::uint64_t... bases2>
        requires(std::ranges::is_sorted(
            std::array<std::uint64_t, sizeof...(bases2)>{bases2...}))
    friend class Multivector;
    // This is the one bit of template metaprogramming used, for converting
    // vectors of basis indices to a multivector with those basis multvectors
    template <
        auto arr,
        typename IS = decltype(std::make_index_sequence<arr().size()>())
    >
    struct from_wrapper_type;
    template <auto arr, std::size_t... I>
    struct from_wrapper_type<arr, std::index_sequence<I...>> {
        using type = Multivector<Scalar, metric, arr()[I]...>;
    };
    template <auto arr>
    using from_wrapper = typename from_wrapper_type<arr>::type;

    template <std::uint64_t basis>
    using BB = BasisBlade<Scalar, metric, basis>;
    template <std::uint64_t... bases2>
    using MV = Multivector<Scalar, metric, bases2...>;

    struct Zero {
        consteval explicit(false) Zero(Scalar value)
        {
            if (value != 0) throw std::runtime_error(
                    "This multivector has no scalar component");
        }
    };

    public:
        /** @brief Default constructor, zero-initializing all coefficients. */
        constexpr Multivector() : BB<bases>()... {}
        /** @brief Constructor that takes in scalar values for every
         * coefficient.
         * @param values A list of @ref BasisBlade "BasisBlade"s to represent
         * all of the coefficients.  Note that because `BasisBlade`s can be
         * constructed implicitly from scalars, it's better to just pass scalars
         * to this constructor.  You must pass in all the coefficients that are
         * going to be present in this multivector.
         */
        constexpr Multivector(BB<bases>... values)
            requires(sizeof...(bases) > 0)
            : BB<bases>(values)... {}
        /** @brief Constructs a multivector from another multivector.
         *
         * All of the components in the other multivector must be valid in this
         * one.  All components in this multivector that are not in the other
         * one are set to zero.
         *
         * @tparam bases2 The bases present in `other`.  If `bases2` is not a
         * subset of `bases`, the program is ill-formed.
         * @param other The multivector to construct this one from.
         */
        template <std::uint64_t... bases2>
        constexpr Multivector(const MV<bases2...>& other)
        {
            ((BB<bases2>::coefficient = other.BB<bases2>::coefficient), ...);
        }
        /** @brief Constructs a multivector from a single scalar value.
         *
         * This allows you to write things like "Rotor r = 1".  This will only
         * be valid in overload resolution if this multivector actually has a
         * scalar component.
         */
        constexpr Multivector(Scalar value)
            requires((bases == 0) or ...)
        {
            BB<0>::coefficient = value;
        }
        /** @brief Construct a multivector from the literal 0
         *
         * The type `Zero` is one that can only be constructed from 0.
         */
        constexpr Multivector(Zero) requires(((bases != 0) and ...)
                and sizeof...(bases) > 1) {}
        /** @brief Assign a multivector to zero
         *
         * The type `Zero` is one that can only be constructed from 0.
         */
        Multivector& operator=(Zero) requires ((bases != 0) and ...)
            {return *this = Multivector();}

        /** @brief Checks if all components are equal. */
        constexpr bool operator==(const Multivector& other) const=default;
        /** @brief Checks if all components are equal. */
        constexpr bool operator!=(const Multivector& other) const=default;
        /** @brief Checks if two multivector of different types are equal
         *
         * For components that are shared by both multivectors, it checks if
         * they are equal.  For components that are not shared, it checks if
         * they are zero.
         */
        template <std::uint64_t... bases2>
        constexpr bool operator==(const MV<bases2...>& other) const
        {
            return ((binary_blade_project<bases>() ==
                    other.template binary_blade_project<bases>()) and ...) and
                   ((binary_blade_project<bases2>() ==
                    other.template binary_blade_project<bases2>()) and ...);
        }
        /** @brief Checks if two multivector of different types are not equal */
        template <std::uint64_t... bases2>
        constexpr bool operator!=(const MV<bases2...>& other) const
        {
            return !(*this == other);
        }
        /** @brief Equality operator for scalars */
        constexpr bool operator==(Scalar s) const
        {
            return binary_blade_project<0>() == s and
                ((binary_blade_project<bases>() == 0 or bases == 0) and ...);
        }
        /** @brief Inequality operator for scalars */
        constexpr bool operator!=(Scalar s) const
        {
            return !(*this == s);
        }
        /** @brief Symmetric version of the above operators */
        constexpr friend bool operator==(Scalar s, const Multivector& m)
        {
            return m == s;
        }
        /** @brief Symmetric version of the above operators */
        constexpr friend bool operator!=(Scalar s, const Multivector& m)
        {
            return m != s;
        }

        /** @brief Computes the projection of the shared components of a
         * multivector onto this multivector
         *
         * An example: Let `a = 2*e1` and let `b = 3*e1 + 4*e12`.  Then after
         * running `a.generic_project(b)`, we have `a == 3*e1`.
         *
         * This function is pretty low-level so you probably won't find much
         * reason to use it yourself.  It's mainly used in defining other
         * operations.  Note that it modifies `*this`!
         */
        template <std::uint64_t... bases2>
        constexpr void generic_project(const MV<bases2...>& b)
        {
            ((BB<bases>::coefficient = b.BB<bases>::coefficient), ...);
        }

        /** @brief Projects the multivector onto a particular blade using the
         * binary representation of it
         *
         * An example: `(2*e1 + 3*e2).blade_project<1> == 2`.  Note that you
         * need to know the binary representation of the blade you are
         * projecting.
         * @tparam basis The binary form of the basis multivector you are
         * projecting onto
         */
        template <std::uint64_t basis>
        constexpr Scalar binary_blade_project() const
        {
            if constexpr ((... || (bases == basis))) {
                return BB<basis>::coefficient;
            }
            else return 0;
        }
        /** @brief Projects the multivector onto a particular blade
         *
         * The basis blade you are projecting onto must be an actual basis
         * blade!  If it's not, the behavior is undefined.  Note that this
         * accepts any scalar multiple of a basis blade, and it will work as you
         * would expect: `v.blade_project<e1>() == 2*v.blade_project<2*e1>()`.
         * Just make sure that there is always exactly one nonzero component in
         * `b`.
         *
         * @tparam b The basis blade you are projecting onto.  If the number of
         * nonzero components in `b` is not equal to one, the behavior is
         * undefined.
         */
        template <auto b>
        Scalar blade_project() const
        {
            constexpr auto basis = b.component();
            return BB<basis>::coefficient / b.BB<basis>::coefficient;
        }

        /** @brief Grade projection
         * @tparam grade The grade you are projecting onto
         */
        template <std::uint64_t grade>
        constexpr auto grade_project() const
        {
            auto result = from_wrapper<[&]{
                auto result = std::vector<std::uint64_t>();
                result.reserve(sizeof...(bases));
                ((std::popcount(bases) == grade ?
                  (result.push_back(bases)) : void()), ...);
                return result;
            }>();
            result.generic_project(*this);
            return result;
        }

        /** @brief Unary plus, which is a no-op here. */
        constexpr auto operator+() const
        {
            return *this;
        }
        /** @brief Addition, modying `*this`
         *
         * @param other The multivector getting added to `*this`.  The bases
         * present in `other` must be a subset of the bases present in `*this`!
         * If you want to add two multivectors that don't share bases, it
         * necessarily produces a new type, so you must use `operator+` in that
         * instance.
         */
        template <std::uint64_t... bases2>
        constexpr auto& operator+=(const MV<bases2...>& other)
        {
            ((BB<bases2>::coefficient +=
                other.BB<bases2>::coefficient), ...);
            return *this;
        }
        /** @brief (Hopefully) an optimization for when adding two multivectors
         * of the same type
         */
        constexpr auto operator+(const Multivector& other) const
        {
            auto result = *this;
            result += other;
            return result;
        }
        /** @brief General addition.
         *
         * Note that in general, this produces a type different than the type of
         * both `*this` and `other`.
         */
        template <std::uint64_t... bases2>
        constexpr auto operator+(const MV<bases2...>& other) const
        {
            auto result = from_wrapper<[&]{
                auto result = std::vector<std::uint64_t>();
                result.reserve(sizeof...(bases) + sizeof...(bases2));
                auto bs1 = std::array<std::uint64_t, sizeof...(bases)>
                    {bases...};
                auto bs2 = std::array<std::uint64_t, sizeof...(bases2)>
                    {bases2...};
                auto i1 = 0UL;
                auto i2 = 0UL;
                while (i1 < bs1.size() and i2 < bs2.size()) {
                    if (bs1[i1] < bs2[i2]) {
                        result.push_back(bs1[i1]);
                        ++i1;
                    }
                    else if (bs2[i2] < bs1[i1]) {
                        result.push_back(bs2[i2]);
                        ++i2;
                    }
                    else {
                        result.push_back(bs1[i1]);
                        ++i1;
                        ++i2;
                    }
                }
                std::copy(
                    bs1.begin() + i1,
                    bs1.end(),
                    std::back_inserter(result)
                );
                std::copy(
                    bs2.begin() + i2,
                    bs2.end(),
                    std::back_inserter(result)
                );
                return result;
            }>();
            result += *this;
            result += other;
            return result;
        }

        constexpr auto operator+=(const Scalar& s)
        {
            *this += MV<0>(s);
        }
        constexpr auto operator+(const Scalar& s) const
        {
            return *this + MV<0>(s);
        }
        constexpr friend auto operator+(const Scalar& s, const Multivector& m)
        {
            return MV<0>(s) + m;
        }
        /** @brief Unary minus, which negates every component */
        constexpr auto operator-() const
        {
            auto result = *this;
            ((result.BB<bases>::coefficient =
                -result.BB<bases>::coefficient), ...);
            return result;
        }
        /** @brief Subtraction, modying `*this`
         *
         * @param other The multivector subtracting from `*this`.  The bases
         * present in `other` must be a subset of the bases present in `*this`!
         * If you want to add two multivectors that don't share bases, it
         * necessarily produces a new type, so you must use `operator-` in that
         * instance.
         */
        template <std::uint64_t... bases2>
        constexpr auto& operator-=(const MV<bases2...>& other)
        {
            ((BB<bases2>::coefficient -=
                other.BB<bases2>::coefficient), ...);
            return *this;
        }
        /** @brief (Hopefully) an optimization for when subtracting two
         * multivectors of the same type
         */
        constexpr auto operator-(const Multivector& other) const
        {
            auto result = *this;
            ((result.BB<bases>::coefficient -=
                other.BB<bases>::coefficient), ...);
            return result;
        }
        /** @brief General subtraction.
         *
         * Note that in general, this produces a type different than the type of
         * both `*this` and `other`.
         */
        template <std::uint64_t... bases2>
        constexpr auto operator-(const MV<bases2...>& other) const
        {
            return *this + (-other);
        }

        constexpr auto operator-=(const Scalar& s)
        {
            *this -= MV<0>(s);
        }
        constexpr auto operator-(const Scalar& s) const
        {
            return *this - MV<0>(s);
        }
        constexpr friend auto operator-(const Scalar& s, const Multivector& m)
        {
            return MV<0>(s) - m;
        }

        /** @brief Calculates the Hodge dual of this multivector
         */
        constexpr auto dual() const
        {
            constexpr auto N = metric.size();
            auto result = from_wrapper<[]{
                auto result = std::vector<std::uint64_t>{bases...};
                for (auto& b : result) {
                    b = b ^ ((1 << N) - 1);
                }
                std::ranges::sort(result);
                return result;
            }>();
            ((result.BB<bases ^ ((1 << N) - 1)>::coefficient =
                BB<bases>::coefficient * dual_sign<N>(bases)), ...);
            return result;
        }
        /** @brief Calculates the inverse of the dual */
        // I know that there are more efficient versions of this but I don't
        // want to bother
        constexpr auto undual() const
        {
            return dual().dual().dual();
        }
        /** @brief Operator version of @ref dual */
        constexpr auto operator!() const
        {
            return dual();
        }

        /** @brief Calculates the reverse of this multivector */
        constexpr Multivector reverse() const
        {
            auto result = *this;
            ((result.BB<bases>::coefficient *=
              ((std::popcount(bases) / 2) % 2 ? -1 : 1)), ...);
            return result;
        }
        /** @brief Operator version of @ref reverse */
        constexpr Multivector operator~() const
        {
            return reverse();
        }
        /** @brief Calculates the grade involution of this multivector */
        constexpr Multivector involute() const
        {
            auto result = *this;
            ((result.BB<bases>::coefficient *=
              (std::popcount(bases) % 2 ? -1 : 1)), ...);
            return result;
        }

        /** @brief Calculates the norm squared of this multivector.
         *
         * Note that there are multiple definitions of this operation in the
         * literature.  The definition used here is `|A|^2 = ⟨A*A†⟩`.
         */
        constexpr Scalar norm2() const
        {
            return (*this * reverse()).template binary_blade_project<0>();
        }
        /** @brief Calculates the norm of this multivector.
         *
         * Like @ref norm2, there are multiple definitions of this operation in
         * the literature.  Here the result is the square root of the absolute
         * value of @ref norm2.  Note that this requires scalars to be able to
         * be passed to `std::sqrt` and `std::abs`.
         */
        constexpr Scalar norm() const
        {
            return std::sqrt(std::abs(norm2()));
        }
        /** @brief Returns a normalized version of this multivector
         *
         * The definition of normalization used here is just `*this / norm()`.
         */
        constexpr Multivector normalized() const
        {
            return *this / norm();
        }

        /** @brief Calculates the geometric product of two multivectors */
        template <std::uint64_t... bases2>
        constexpr auto operator*(const MV<bases2...>& b) const
        {
            if constexpr (sizeof...(bases) == 0) return *this;
            else if constexpr (sizeof...(bases2) == 0) return b;
            else return generic_mult<[](auto, auto){return true;}>(b);
        }
        /** @brief Right-multiplies this multivector by another */
        template <std::uint64_t... bases2>
        constexpr Multivector& operator*=(const MV<bases2...>& b)
        {
            *this = *this * b;
            return *this;
        }

        /** @brief Multiplies this multivector by a scalar */
        constexpr auto operator*=(const Scalar& s)
        {
            ((BB<bases>::coefficient *= s), ...);
        }
        /** @brief Divides this multivector by a scalar */
        constexpr auto operator/=(const Scalar& s)
        {
            ((BB<bases>::coefficient /= s), ...);
        }
        /** @brief Returns the product of this multivector and a scalar */
        constexpr auto operator*(const Scalar& s) const
        {
            auto result = *this;
            result *= s;
            return result;
        }
        /** @brief Returns the quotient of this multivector and a scalar */
        constexpr auto operator/(const Scalar& s) const
        {
            auto result = *this;
            result /= s;
            return result;
        }
        /** @brief Returns the product of a scalar and a multivector */
        constexpr friend auto operator*(const Scalar& s, const Multivector& m)
        {
            return m * s;
        }

        /** @brief Calculates the outer/wedge/exterior product of two
         * multivectors */
        template <std::uint64_t... bases2>
        constexpr auto operator^(const MV<bases2...>& b) const
        {
            return generic_mult<[](std::uint64_t m, std::uint64_t n){
                return (m & n) == 0;
            }>(b);
        }
        /** @brief Calculates the inner product of two multivectors
         *
         * For a j-vector A and a k-vector B, `A|B = ⟨A*B⟩_|j-k|`.
         */
        template <std::uint64_t... bases2>
        constexpr auto operator|(const MV<bases2...>& b) const
        {
            return generic_mult<[](std::uint64_t m, std::uint64_t n){
                return (m | n) == m or (m | n) == n;
            }>(b);
        }
        /** @brief Calculates the left contraction of two multivectors
         *
         * For a j-vector A and a k-vector B, `A<<B = ⟨A*B⟩_(k-j)`.
         */
        template <std::uint64_t... bases2>
        constexpr auto operator<<(const MV<bases2...>& b) const
        {
            return generic_mult<[](std::uint64_t m, std::uint64_t n){
                return (m | n) == n;
            }>(b);
        }
        /** @brief Calculates the right contraction of two multivectors
         *
         * For a j-vector A and a k-vector B, `A>>B = ⟨A*B⟩_(j-k)`.
         */
        template <std::uint64_t... bases2>
        constexpr auto operator>>(const MV<bases2...>& b) const
        {
            return generic_mult<[](std::uint64_t m, std::uint64_t n){
                return (m | n) == m;
            }>(b);
        }
        /** @brief Calculates the regressive product of two multivectors */
        template <std::uint64_t... bases2>
        constexpr auto operator&(const MV<bases2...>& b) const
        {
            return (dual() ^ b.dual()).undual();
        }
        /** @brief Special case of the outer product with an empty multivector
         */
        constexpr auto operator^(const MV<>& b) const
            {return b;}
        /** @brief Special case of the inner product with an empty multivector
         */
        constexpr auto operator|(const MV<>& b) const
            {return b;}
        /** @brief Special case of the left contraction with an empty
         * multivector
         */
        constexpr auto operator<<(const MV<>& b) const
            {return b;}
        /** @brief Special case of the right contraction with an empty
         * multivector
         */
        constexpr auto operator>>(const MV<>& b) const
            {return b;}
        /** @brief Special case of the regressive product with an empty
         * multivector
         */
        constexpr auto operator&(const MV<>& b) const
            {return b;}
        /** @brief Special case of the outer product with an empty multivector
         */
        constexpr friend auto operator^(const MV<>& b, const Multivector&)
            {return b;}
        /** @brief Special case of the inner product with an empty multivector
         */
        constexpr friend auto operator|(const MV<>& b, const Multivector&)
            {return b;}
        /** @brief Special case of the left contraction with an empty
         * multivector
         */
        constexpr friend auto operator<<(const MV<>& b, const Multivector&)
            {return b;}
        /** @brief Special case of the right contraction with an empty
         * multivector
         */
        constexpr friend auto operator>>(const MV<>& b, const Multivector&)
            {return b;}
        /** @brief Special case of the regressive product with an empty
         * multivector
         */
        constexpr friend auto operator&(const MV<>& b, const Multivector&)
            {return b;}

    private:
        // I know I promised that the above was the only template
        // metaprogramming, but this is basically the same thing
        template <
            auto arr,
            typename IS = decltype(std::make_index_sequence<arr().size()>())
        >
        struct gm_calc;
        template <auto arr, std::size_t... I>
        struct gm_calc<arr, std::index_sequence<I...>> {
            template <
                std::pair<std::uint64_t, std::uint64_t>... bs,
                typename R,
                typename A,
                typename B
            >
            constexpr static void real_calc(R& res, const A& a, const B& b)
            {
                ((res.BB<bs.first ^ bs.second>::coefficient +=
                    basis_blade_product_sign<
                        Scalar, bs.first, bs.second>(metric) *
                    a.BB<bs.first>::coefficient * b.BB<bs.second>::coefficient),
                 ...);
            }
            template <
                typename R,
                typename A,
                typename B
            >
            constexpr static void calc(R& res, const A& a, const B& b)
            {
                real_calc<arr()[I]...>(res, a, b);
            }
        };
        template <auto P, std::uint64_t... bases2>
        constexpr auto generic_mult(const MV<bases2...>& b) const
        {
            constexpr auto pairs = [&]{
                auto result
                    = std::vector<std::pair<std::uint64_t, std::uint64_t>>();
                result.reserve(sizeof...(bases) * sizeof...(bases2));
                for (auto i1 : {bases...}) {
                    for (auto i2 : {bases2...}) {
                        if (P(i1, i2)) {
                            result.emplace_back(i1, i2);
                        }
                    }
                }
                return result;
            };
            auto result = from_wrapper<[&]{
                auto result = std::vector<std::uint64_t>();
                result.reserve(sizeof...(bases) * sizeof...(bases2));
                for (auto i1 : {bases...}) {
                    for (auto i2 : {bases2...}) {
                        if (P(i1, i2)) {
                            result.push_back(i1 ^ i2);
                        }
                    }
                }
                std::ranges::sort(result);
                auto ret = std::ranges::unique(result);
                result.erase(ret.begin(), ret.end());
                return result;
            }>();
            gm_calc<pairs>::calc(result, *this, b);
            return result;
        }
        constexpr std::uint64_t component() const
        {
            return ((BB<bases>::coefficient ? bases : 0) + ...);
        }
};

}

#endif
