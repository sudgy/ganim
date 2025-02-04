#ifndef GANIM_UTIL_TRIANGULATION_HPP
#define GANIM_UTIL_TRIANGULATION_HPP

#include "ganim/ga/vga2.hpp"
#include "ganim/ga/pga2.hpp"

namespace ganim {
    /** @brief Triangulate an arbitrary polygon.
     *
     * This function will first determine if the polygon is convex or concave
     * and will then use that to figure out which polygon triangulation
     * algorithm to run.  If you know beforehand if your polygon is convex or
     * concave, use @ref triangulate_convex or @ref triangulate_concave instead.
     */
    std::vector<unsigned> triangulate(
        const std::vector<vga2::Vec>& polygon
    );
    /** @brief Triangulate an arbitrary polygon.
     *
     * This function will first determine if the polygon is convex or concave
     * and will then use that to figure out which polygon triangulation
     * algorithm to run.  If you know beforehand if your polygon is convex or
     * concave, use @ref triangulate_convex or @ref triangulate_concave instead.
     */
    std::vector<unsigned> triangulate(
        const std::vector<pga2::Bivec>& polygon
    );
    /** @brief Triangulate a convex polygon. */
    std::vector<unsigned> triangulate_convex(
        const std::vector<vga2::Vec>& polygon
    );
    /** @brief Triangulate a convex polygon. */
    std::vector<unsigned> triangulate_convex(
        const std::vector<pga2::Bivec>& polygon
    );
    /** @brief Triangulate a concave polygon. */
    std::vector<unsigned> triangulate_concave(
        const std::vector<vga2::Vec>& polygon
    );
    /** @brief Triangulate a concave polygon. */
    std::vector<unsigned> triangulate_concave(
        const std::vector<pga2::Bivec>& polygon
    );
}

#endif
