#include "arc.hpp"

#include "ganim/ga/exp.hpp"

using namespace ganim;

using namespace vga2;

namespace {
    std::vector<Vec> get_points(
        double start_angle,
        double end_angle,
        double radius
    )
    {
        auto result = std::vector<Vec>();
        result.reserve(128);
        auto start = e1 * ga_exp(e12*start_angle) * radius;
        auto angle = end_angle - start_angle;
        for (int i = 0; i <= 128; ++i) {
            result.push_back(start * ga_exp(e12*angle*i/128.0));
        }
        return result;
    }
}

Arc::Arc(double start_angle, double end_angle, double radius, double thickness)
:   Path(get_points(start_angle, end_angle, radius), false, thickness) {}
