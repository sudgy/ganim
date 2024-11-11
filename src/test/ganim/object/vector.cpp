#include <catch2/catch_test_macros.hpp>
#include "test/ganim/ga_equals.hpp"

#include "ganim/object/vector.hpp"

#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("Vector object construction", "[object]") {
    using namespace pga3;
    auto test1 = ganim::Vector(e1);
    auto test2 = ganim::Vector(e2);
    auto test3 = ganim::Vector(e1 + e2);
    auto test4 = ganim::Vector(e1 + e3);
    auto test5 = ganim::Vector(e3);
    auto test6 = ganim::Vector(e1, 2*e1);
    auto test7 = ganim::Vector(e1, e1 + e3);

    REQUIRE_THAT(test1.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test2.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test3.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test4.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test5.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test6.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test7.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));

    REQUIRE_THAT(test1.get_end_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test2.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));
    REQUIRE_THAT(test3.get_end_pga3().undual(), GAEquals(e0 + e1 + e2, 1e-5));
    REQUIRE_THAT(test4.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));
    REQUIRE_THAT(test5.get_end_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    REQUIRE_THAT(test6.get_end_pga3().undual(), GAEquals(e0 + 2*e1, 1e-5));
    REQUIRE_THAT(test7.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));

    auto r1 = test1.get_rotor();
    auto r2 = test2.get_rotor();
    auto r3 = test3.get_rotor();
    auto r4 = test4.get_rotor();
    auto r5 = test5.get_rotor();
    auto r6 = test6.get_rotor();
    auto r7 = test7.get_rotor();

    auto down = (-e2 + e0).dual();
    REQUIRE_THAT((~r1 * down * r1).undual(), GAEquals(e0 - e2, 1e-5));
    REQUIRE_THAT((~r2 * down * r2).undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT((~r3 * down * r3).undual(),
            GAEquals(e0 + (e1 - e2)/std::sqrt(2), 1e-5));
    REQUIRE_THAT((~r4 * down * r4).undual(), GAEquals(e0 - e2, 1e-5));
    REQUIRE_THAT((~r5 * down * r5).undual(), GAEquals(e0 - e2, 1e-5));
    REQUIRE_THAT((~r6 * down * r6).undual(), GAEquals(e0 + e1 - e2, 1e-5));
    REQUIRE_THAT((~r7 * down * r7).undual(), GAEquals(e0 + e1 - e2, 1e-5));
}

TEST_CASE("Vector object scale", "[object]") {
    using namespace vga3;
    auto test = ganim::Vector(e1);
    test.scale(2);
    REQUIRE_THAT(test.get_start_vga3(), GAEquals(0));
    REQUIRE_THAT(test.get_end_vga3(), GAEquals(2*e1));
    test.scale(e1, 2);
    REQUIRE_THAT(test.get_start_vga3(), GAEquals(-e1));
    REQUIRE_THAT(test.get_end_vga3(), GAEquals(3*e1));
}

TEST_CASE("Vector object transforms", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(e1);
    test.rotate(e12, τ/4);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));

    test.rotate(e23, τ/4);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    auto r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + e1, 1e-5));

    test.shift(e1);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + 2*e1, 1e-5));

    test.rotate(e13, τ/4);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e1 + e3, 1e-5));
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + e2+e3, 1e-5));
}

TEST_CASE("Vector object zero vector", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(0*e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.set_end(e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e1));
    test.set_end(0*e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
}
