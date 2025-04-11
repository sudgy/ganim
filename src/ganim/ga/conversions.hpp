#ifndef GANIM_GA_CONVERSIONS_HPP
#define GANIM_GA_CONVERSIONS_HPP

/** @file
 * @brief Contains various conversion functions between different algebras
 *
 * While most of these are straightforward, there are a few that aren't.  When
 * converting from 2D to 3D, do you extend an object into the third dimension or
 * leave it flat?  Do VGA vectors represent points or lines?  In cases where it
 * seems ambiguous, I provided multiple functions with slightly different names.
 * Refer to the documentation for the individual functions to figure out exactly
 * what choices I made.
 */

#include "vga2.hpp"
#include "vga3.hpp"
#include "pga2.hpp"
#include "pga3.hpp"

namespace ganim {

/** @brief Convert a 2D VGA scalar to a 3D VGA scalar */
vga3::Scalar vga2_to_vga3(const vga2::Scalar& in);
/** @brief Convert a 2D VGA vector to a 3D VGA vector
 *
 * This produces the same geometric object.
 */
vga3::Vec vga2_to_vga3(const vga2::Vec& in);
/** @brief Convert a 2D VGA bivector to a 3D VGA bivector
 *
 * This produces the same geometric object.
 */
vga3::Bivec vga2_to_vga3(const vga2::Bivec& in);
/** @brief Convert a 2D VGA even element to a 3D VGA even element.
 *
 * This produces the same geometric object.
 */
vga3::Even vga2_to_vga3(const vga2::Even& in);

#define DEFINE_VECTOR_CONVERTER(conversion_func) \
template <typename T> \
auto conversion_func(const std::vector<T>& in) \
{ \
    auto result = std::vector<decltype(conversion_func(in[0]))>(); \
    result.reserve(in.size()); \
    for (const auto& a : in) { \
        result.push_back(conversion_func(a)); \
    } \
    return result; \
}

DEFINE_VECTOR_CONVERTER(vga2_to_vga3);

/** @brief Convert a 3D VGA scalar to a 2D VGA scalar */
vga2::Scalar vga3_to_vga2(const vga3::Scalar& in);
/** @brief Convert a 3D VGA vector to a 2D VGA vector
 *
 * This projects the vector onto the xy-plane.
 */
vga2::Vec vga3_to_vga2(const vga3::Vec& in);
/** @brief Convert a 3D VGA bivector to a 2D VGA bivector
 *
 * This projects the bivector onto the xy-plane.
 */
vga2::Bivec vga3_to_vga2(const vga3::Bivec& in);

DEFINE_VECTOR_CONVERTER(vga3_to_vga2);

/** @brief Convert a 2D PGA scalar to a 3D PGA scalar */
pga3::Scalar pga2_to_pga3(const pga2::Scalar& in);
/** @brief Convert a 2D PGA line to a 3D PGA plane
 *
 * This produces the vertical plane going through the input line.
 */
pga3::Vec pga2_to_pga3(const pga2::Vec& in);
/** @brief Convert a 2D PGA point to a 3D PGA line
 *
 * This produces the vertical line going through the input point.
 */
pga3::Bivec pga2_to_pga3(const pga2::Bivec& in);
/** @brief Convert a 2D PGA even element to a 3D PGA even element
 *
 * This produces the same geometric transformation as the input.
 */
pga3::Even pga2_to_pga3(const pga2::Even& in);
/** @brief Convert a 2D PGA line to a 3D PGA line.
 *
 * This produces the same geometric object.
 */
pga3::Bivec pga2_to_pga3_flat(const pga2::Vec& in);
/** @brief Convert a 2D PGA point to a 3D PGA point.
 *
 * This produces the same geometric object.
 */
pga3::Trivec pga2_to_pga3_flat(const pga2::Bivec& in);

DEFINE_VECTOR_CONVERTER(pga2_to_pga3);

/** @brief Convert a 3D PGA scalar to a 2D PGA scalar */
pga2::Scalar pga3_to_pga2(const pga3::Scalar& in);
/** @brief Convert a 3D PGA plane to a 2D PGA line
 *
 * This finds the intersection of the input plane with the xy-plane.
 */
pga2::Vec pga3_to_pga2(const pga3::Vec& in);
/** @brief Convert a 3D PGA line to a 2D PGA point
 *
 * This finds the intersection of the input line with the xy-plane.
 */
pga2::Bivec pga3_to_pga2(const pga3::Bivec& in);
/** @brief Convert a 3D PGA point to a 2D PGA point
 *
 * This projects the input point onto the xy-plane.
 */
pga2::Bivec pga3_to_pga2(const pga3::Trivec& in);

DEFINE_VECTOR_CONVERTER(pga3_to_pga2);

/** @brief Convert a 2D VGA scalar to a 2D PGA scalar */
pga2::Scalar vga2_to_pga2(const vga2::Scalar& in);
/** @brief Convert a 2D VGA vector to a 2D PGA point
 *
 * The resulting point is the point that the vector points at when starting at
 * the origin.
 */
pga2::Bivec vga2_to_pga2(const vga2::Vec& in);
/** @brief Convert a 2D VGA even element to a 2D PGA even element
 *
 * This produces the same geometric transformation.
 */
pga2::Even vga2_to_pga2(const vga2::Even& in);
/** @brief Convert a 2D VGA vector to a 2D PGA line
 *
 * This produces the line the vector goes through.
 */
pga2::Vec vga2_to_pga2_full(const vga2::Vec& in);

DEFINE_VECTOR_CONVERTER(vga2_to_pga2);

/** @brief Convert a 2D PGA scalar to a 2D VGA scalar */
vga2::Scalar pga2_to_vga2(const pga2::Scalar& in);
/** @brief Convert a 2D PGA point to a 2D VGA vector
 *
 * If the input point is ideal, it interprets the coefficients as the
 * coefficients of a VGA vector.  Otherwise, it produces the vector going from
 * the origin to the input point.
 */
vga2::Vec pga2_to_vga2(const pga2::Bivec& in);
/** @brief Interpret the components of a 2D PGA vector as a 2D VGA vector
 *
 * Any e0 component will be ignored.
 */
vga2::Vec pga2_to_vga2_cheat(const pga2::Vec& in);

DEFINE_VECTOR_CONVERTER(pga2_to_vga2);

/** @brief Convert a 3D VGA scalar to a 3D PGA scalar */
pga3::Scalar vga3_to_pga3(const vga3::Scalar& in);
/** @brief Convert a 3D VGA vector to a 3D PGA point
 *
 * The resulting point is the point that the vector points at when starting at
 * the origin.
 */
pga3::Trivec vga3_to_pga3(const vga3::Vec& in);
/** @brief Convert a 3D VGA bivector to a 3D PGA plane
 *
 * The resulting plane is the plane of the bivector.
 */
pga3::Vec vga3_to_pga3(const vga3::Bivec& in);
/** @brief Convert a 3D VGA even element to a 3D PGA even element
 *
 * This produces the same geometric transformation.
 */
pga3::Even vga3_to_pga3(const vga3::Even& in);
/** @brief Convert a 3D VGA vec to a 3D PGA line
 *
 * This produces the line the vec goes through.
 */
pga3::Bivec vga3_to_pga3_full(const vga3::Vec& in);

DEFINE_VECTOR_CONVERTER(vga3_to_pga3);

/** @brief Convert a 3D PGA scalar to a 3D VGA scalar */
vga3::Scalar pga3_to_vga3(const pga3::Scalar& in);
/** @brief Convert a 3D PGA point to a 3D VGA vector
 *
 * If the input point is ideal, it interprets the coefficients as the
 * coefficients of a VGA vector.  Otherwise, it produces the vector going from
 * the origin to the input point.
 */
vga3::Vec pga3_to_vga3(const pga3::Trivec& in);
/** @brief Interpret the components of a 3D PGA vector as a 3D VGA vector
 *
 * Any e0 component will be ignored.
 */
vga3::Vec pga3_to_vga3_cheat(const pga3::Vec& in);

DEFINE_VECTOR_CONVERTER(pga3_to_vga3);

/** @brief Convert a 3D PGA multivector to a 2D VGA multivector
 *
 * This is just the composition of @ref vga3_to_vga2 and @ref pga3_to_vga3.  If
 * manually calling those two functions is ill-formed on the input multivector,
 * this function is ill-formed too.
 */
template <std::uint64_t... bases>
auto pga3_to_vga2(const pga3::Multivector<bases...>& in)
{
    return vga3_to_vga2(pga3_to_vga3(in));
}

/** @brief Convert a 2D VGA scalar to a 3D PGA scalar */
pga3::Scalar vga2_to_pga3(const vga2::Scalar& in);
/** @brief Convert a 2D VGA vector to a 3D PGA point
 *
 * The resulting point is the point that the vector points at when starting at
 * the origin.
 */
pga3::Trivec vga2_to_pga3(const vga2::Vec& in);
/** @brief Convert a 3D VGA even element to a 3D PGA even element
 *
 * This produces the same geometric transformation.
 */
pga3::Even vga2_to_pga3(const vga2::Even& in);
/** @brief Convert a 2D VGA vector to a 3D PGA line
 *
 * This produces the line the vector goes through.
 */
pga3::Bivec vga2_to_pga3_full(const vga2::Vec& in);

DEFINE_VECTOR_CONVERTER(vga2_to_pga3);

#undef DEFINE_VECTOR_CONVERTER

/** @brief A lot of functions take in a point in space, and since there are
 * several different representations of points in space, this concept checks if
 * it's one of the ones that I consider a valid representation of a point.  They
 * are:
 *  - A 2D VGA vector
 *  - A 3D VGA vector
 *  - A 2D PGA bivector
 *  - A 3D PGA trivector
 *  - A 2D PGA vector, which you just interpret as a 2D VGA vector
 *  - A 3D PGA vector, which you just interpret as a 3D VGA vector
 *
 *  If you want to write a function that takes in a point, just write one that
 *  takes in a trivector, and another one with this concept that just uses @ref
 *  ganim::pointlike_to_pga3 pointlike_to_pga3.
 */
template <typename T>
concept pointlike = std::same_as<T, vga2::Vec> or
                    std::same_as<T, vga3::Vec> or
                    std::same_as<T, pga2::Vec> or
                    std::same_as<T, pga3::Vec> or
                    std::same_as<T, pga2::Bivec> or
                    std::same_as<T, pga3::Trivec>;

/** @brief Convert one of the pointlike types to a 3D PGA trivector.  */
pga3::Trivec pointlike_to_pga3(const pointlike auto& p)
{
    using T = std::remove_cvref_t<decltype(p)>;
    if constexpr (std::is_same_v<T, vga2::Vec>) {
        return vga2_to_pga3(p);
    }
    else if constexpr (std::is_same_v<T, vga3::Vec>) {
        return vga3_to_pga3(p);
    }
    else if constexpr (std::is_same_v<T, pga2::Vec>) {
        return vga2_to_pga3(pga2_to_vga2_cheat(p));
    }
    else if constexpr (std::is_same_v<T, pga3::Vec>) {
        return vga3_to_pga3(pga3_to_vga3_cheat(p));
    }
    else if constexpr (std::is_same_v<T, pga2::Bivec>) {
        return pga2_to_pga3_flat(p);
    }
    else return p;
}

}

#endif
