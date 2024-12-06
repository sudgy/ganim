#ifndef GANIM_UTIL_TRIANGULATION_HPP
#define GANIM_UTIL_TRIANGULATION_HPP

#include "ganim/ga/vga2.hpp"
#include "ganim/ga/pga2.hpp"

namespace ganim {
    std::vector<unsigned> triangulate(
        const std::vector<vga2::Vec>& polygon
    );
    std::vector<unsigned> triangulate(
        const std::vector<pga2::Bivec>& polygon
    );
    std::vector<unsigned> triangulate_convex(
        const std::vector<vga2::Vec>& polygon
    );
    std::vector<unsigned> triangulate_convex(
        const std::vector<pga2::Bivec>& polygon
    );
    std::vector<unsigned> triangulate_concave(
        const std::vector<vga2::Vec>& polygon
    );
    std::vector<unsigned> triangulate_concave(
        const std::vector<pga2::Bivec>& polygon
    );
}

#endif
