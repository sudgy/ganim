#include <catch2/catch_test_macros.hpp>

#include "ganim/util/triangle_rotor.hpp"

#include "test/ganim/ga_equals.hpp"

using namespace ganim;
using namespace pga3;

TEST_CASE("triangle_rotor", "[util]") {
    auto test = [](
        int num,
        Trivector a1,
        Trivector a2,
        Trivector a3,
        Trivector b1,
        Trivector b2,
        Trivector b3
    ) {
        INFO("Number " << num);
        auto r = triangle_rotor(a1, a2, a3, b1, b2, b3);
        REQUIRE_THAT((~r*a1*r).undual(), GAEquals(b1.undual(), 1e-5));
        REQUIRE_THAT((~r*a2*r).undual(), GAEquals(b2.undual(), 1e-5));
        REQUIRE_THAT((~r*a3*r).undual(), GAEquals(b3.undual(), 1e-5));
    };
    test(
        0,
        (e0).dual(),
        (e1 + e0).dual(),
        (e2 + e0).dual(),
        (e0).dual(),
        (e1 + e0).dual(),
        (e2 + e0).dual()
    );
    test(
        1,
        (e0).dual(),
        (e1 + e0).dual(),
        (e2 + e0).dual(),
        (e0).dual(),
        (e2 + e0).dual(),
        (e1 + e0).dual()
    );
    test(
        2,
        (e0).dual(),
        (e1 + e0).dual(),
        (e2 + e0).dual(),
        (e0).dual(),
        (e1 + e0).dual(),
        (e3 + e0).dual()
    );
    test(
        3,
        (e0).dual(),
        (e1 + e0).dual(),
        (e2 + e0).dual(),
        (e1 + e0).dual(),
        (2*e1 + e0).dual(),
        (e1 + e2 + e0).dual()
    );
    test(
        4,
        (e0).dual(),
        (e1 + e0).dual(),
        (e2 + e0).dual(),
        (e0).dual(),
        (-e1 + e0).dual(),
        (e2 + e0).dual()
    );
}
