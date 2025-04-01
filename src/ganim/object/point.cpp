#include "point.hpp"

#include "ganim/math.hpp"
#include "ganim/ga/exp.hpp"

using namespace ganim;

namespace {
    std::vector<vga2::Vec> get_vertices(double radius)
    {
        using namespace vga2;
        auto result = std::vector<vga2::Vec>();
        result.resize(32);
        for (int i = 0; i < 32; ++i) {
            result[i] = radius * e1 * ga_exp(e12*i*τ/32);
        }
        return result;
    }
}

Point::Point(vga2::Vec p, double radius)
    : PolygonShape(::get_vertices(radius))
{
    shift(p);
}

Point::Point(pga2::Bivec p, double radius)
    : PolygonShape(::get_vertices(radius))
{
    shift(p);
}
