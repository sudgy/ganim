#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ganim/scene/camera.hpp"

using namespace ganim;
using namespace Catch::Matchers;

TEST_CASE("Camera", "[scene]") {
    using namespace pga3;
    auto test = Camera(20, 16, 9);
    auto r = test.get_rotor();
    auto p = (r * (8*e1 + 4.5*e2 + e0).dual() * ~r).grade_project<3>();
    auto p2 = ((e123 & p) ^ (e3 + e0)).undual();
    p2 /= p2.blade_project<e0>();
    REQUIRE_THAT(p2.blade_project<e1>() * test.get_x_scale(),
            WithinAbs(1, 1e-5));
    REQUIRE_THAT(p2.blade_project<e2>() * test.get_y_scale(),
            WithinAbs(1, 1e-5));

    test = Camera(10, 9, 16);
    test.shift(e1 + e2 + e3);
    r = test.get_rotor();
    p = (r * (5.5*e1 + 9*e2 + e3 + e0).dual() * ~r).grade_project<3>();
    p2 = ((e123 & p) ^ (e3 + e0)).undual();
    p2 /= p2.blade_project<e0>();
    REQUIRE_THAT(p2.blade_project<e1>() * test.get_x_scale(),
            WithinAbs(1, 1e-5));
    REQUIRE_THAT(p2.blade_project<e2>() * test.get_y_scale(),
            WithinAbs(1, 1e-5));
}
