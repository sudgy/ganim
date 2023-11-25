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
vga3::Vector vga2_to_vga3(const vga2::Vector& in);
/** @brief Convert a 2D VGA bivector to a 3D VGA bivector
 *
 * This produces the same geometric object.
 */
vga3::Bivector vga2_to_vga3(const vga2::Bivector& in);
/** @brief Convert a 2D VGA even element to a 3D VGA even element.
 *
 * This produces the same geometric object.
 */
vga3::Even vga2_to_vga3(const vga2::Even& in);

/** @brief Convert a 3D VGA scalar to a 2D VGA scalar */
vga2::Scalar vga3_to_vga2(const vga3::Scalar& in);
/** @brief Convert a 3D VGA vector to a 2D VGA vector
 *
 * This projects the vector onto the xy-plane.
 */
vga2::Vector vga3_to_vga2(const vga3::Vector& in);
/** @brief Convert a 3D VGA bivector to a 2D VGA bivector
 *
 * This projects the bivector onto the xy-plane.
 */
vga2::Bivector vga3_to_vga2(const vga3::Bivector& in);

/** @brief Convert a 2D PGA scalar to a 3D PGA scalar */
pga3::Scalar pga2_to_pga3(const pga2::Scalar& in);
/** @brief Convert a 2D PGA line to a 3D PGA plane
 *
 * This produces the vertical plane going through the input line.
 */
pga3::Vector pga2_to_pga3(const pga2::Vector& in);
/** @brief Convert a 2D PGA point to a 3D PGA line
 *
 * This produces the vertical line going through the input point.
 */
pga3::Bivector pga2_to_pga3(const pga2::Bivector& in);
/** @brief Convert a 2D PGA even element to a 3D PGA even element
 *
 * This produces the same geometric transformation as the input.
 */
pga3::Even pga2_to_pga3(const pga2::Even& in);
/** @brief Convert a 2D PGA line to a 3D PGA line.
 *
 * This produces the same geometric object.
 */
pga3::Bivector pga2_to_pga3_flat(const pga2::Vector& in);
/** @brief Convert a 2D PGA point to a 3D PGA point.
 *
 * This produces the same geometric object.
 */
pga3::Trivector pga2_to_pga3_flat(const pga2::Bivector& in);

/** @brief Convert a 3D PGA scalar to a 2D PGA scalar */
pga2::Scalar pga3_to_pga2(const pga3::Scalar& in);
/** @brief Convert a 3D PGA plane to a 2D PGA line
 *
 * This finds the intersection of the input plane with the xy-plane.
 */
pga2::Vector pga3_to_pga2(const pga3::Vector& in);
/** @brief Convert a 3D PGA line to a 2D PGA point
 *
 * This finds the intersection of the input line with the xy-plane.
 */
pga2::Bivector pga3_to_pga2(const pga3::Bivector& in);
/** @brief Convert a 3D PGA point to a 2D PGA point
 *
 * This projects the input point onto the xy-plane.
 */
pga2::Bivector pga3_to_pga2(const pga3::Trivector& in);

/** @brief Convert a 2D VGA scalar to a 2D PGA scalar */
pga2::Scalar vga2_to_pga2(const vga2::Scalar& in);
/** @brief Convert a 2D VGA vector to a 2D PGA point
 *
 * The resulting point is the point that the vector points at when starting at
 * the origin.
 */
pga2::Bivector vga2_to_pga2(const vga2::Vector& in);
/** @brief Convert a 2D VGA even element to a 2D PGA even element
 *
 * This produces the same geometric transformation.
 */
pga2::Even vga2_to_pga2(const vga2::Even& in);
/** @brief Convert a 2D VGA vector to a 2D PGA line
 *
 * This produces the line the vector goes through.
 */
pga2::Vector vga2_to_pga2_full(const vga2::Vector& in);

/** @brief Convert a 2D PGA scalar to a 2D VGA scalar */
vga2::Scalar pga2_to_vga2(const pga2::Scalar& in);
/** @brief Convert a 2D PGA point to a 2D VGA vector
 *
 * If the input point is ideal, it interprets the coefficients as the
 * coefficients of a VGA vector.  Otherwise, it produces the vector going from
 * the origin to the input point.
 */
vga2::Vector pga2_to_vga2(const pga2::Bivector& in);
/** @brief Interpret the components of a 2D PGA vector as a 2D VGA vector
 *
 * Any e0 component will be ignored.
 */
vga2::Vector pga2_to_vga2_cheat(const pga2::Vector& in);

/** @brief Convert a 3D VGA scalar to a 3D PGA scalar */
pga3::Scalar vga3_to_pga3(const vga3::Scalar& in);
/** @brief Convert a 3D VGA vector to a 3D PGA point
 *
 * The resulting point is the point that the vector points at when starting at
 * the origin.
 */
pga3::Trivector vga3_to_pga3(const vga3::Vector& in);
/** @brief Convert a 3D VGA bivector to a 3D PGA plane
 *
 * The resulting plane is the plane of the bivector.
 */
pga3::Vector vga3_to_pga3(const vga3::Bivector& in);
/** @brief Convert a 3D VGA even element to a 3D PGA even element
 *
 * This produces the same geometric transformation.
 */
pga3::Even vga3_to_pga3(const vga3::Even& in);
/** @brief Convert a 3D VGA vector to a 3D PGA line
 *
 * This produces the line the vector goes through.
 */
pga3::Bivector vga3_to_pga3_full(const vga3::Vector& in);

/** @brief Convert a 3D PGA scalar to a 3D VGA scalar */
vga3::Scalar pga3_to_vga3(const pga3::Scalar& in);
/** @brief Convert a 3D PGA point to a 3D VGA vector
 *
 * If the input point is ideal, it interprets the coefficients as the
 * coefficients of a VGA vector.  Otherwise, it produces the vector going from
 * the origin to the input point.
 */
vga3::Vector pga3_to_vga3(const pga3::Trivector& in);
/** @brief Interpret the components of a 3D PGA vector as a 3D VGA vector
 *
 * Any e0 component will be ignored.
 */
vga3::Vector pga3_to_vga3_cheat(const pga3::Vector& in);

/** @brief Convert a 2D VGA scalar to a 3D PGA scalar */
pga3::Scalar vga2_to_pga3(const vga2::Scalar& in);
/** @brief Convert a 2D VGA vector to a 3D PGA point
 *
 * The resulting point is the point that the vector points at when starting at
 * the origin.
 */
pga3::Trivector vga2_to_pga3(const vga2::Vector& in);
/** @brief Convert a 3D VGA even element to a 3D PGA even element
 *
 * This produces the same geometric transformation.
 */
pga3::Even vga2_to_pga3(const vga2::Even& in);
/** @brief Convert a 2D VGA vector to a 3D PGA line
 *
 * This produces the line the vector goes through.
 */
pga3::Bivector vga2_to_pga3_full(const vga2::Vector& in);

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
concept pointlike = std::same_as<T, vga2::Vector> or
                    std::same_as<T, vga3::Vector> or
                    std::same_as<T, pga2::Vector> or
                    std::same_as<T, pga3::Vector> or
                    std::same_as<T, pga2::Bivector> or
                    std::same_as<T, pga3::Trivector>;

/** @brief Convert one of the pointlike types to a 3D PGA trivector.  */
pga3::Trivector pointlike_to_pga3(const pointlike auto& p)
{
    using T = std::remove_cvref_t<decltype(p)>;
    if constexpr (std::is_same_v<T, vga2::Vector>) {
        return vga2_to_pga3(p);
    }
    else if constexpr (std::is_same_v<T, vga3::Vector>) {
        return vga3_to_pga3(p);
    }
    else if constexpr (std::is_same_v<T, pga2::Vector>) {
        return vga2_to_pga3(pga2_to_vga2_cheat(p));
    }
    else if constexpr (std::is_same_v<T, pga3::Vector>) {
        return vga3_to_pga3(pga3_to_vga3_cheat(p));
    }
    else if constexpr (std::is_same_v<T, pga2::Bivector>) {
        return pga2_to_pga3_flat(p);
    }
    else return p;
}

}

#endif
