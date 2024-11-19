#ifndef GANIM_UTIL_TRIANGULATION_HPP
#define GANIM_UTIL_TRIANGULATION_HPP

#include "ganim/ga/vga2.hpp"
#include "ganim/ga/pga2.hpp"

namespace ganim {
    std::vector<unsigned> triangulate(
        const std::vector<vga2::Vector>& polygon
    );
    std::vector<unsigned> triangulate(
        const std::vector<pga2::Bivector>& polygon
    );
}

#endif
