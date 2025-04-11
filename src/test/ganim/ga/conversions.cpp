#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"

#include "ganim/ga/conversions.hpp"

using namespace ganim;

TEST_CASE("vga2_to_vga3", "[ga]") {
    auto test1 = vga2::Scalar(2);
    auto test2 = 3*vga2::e1 + 2*vga2::e2;
    auto test3 = 2*vga2::e12;
    auto test4 = 1 + 2*vga2::e12;
    using namespace vga3;
    REQUIRE_THAT(vga2_to_vga3(test1), GAEquals(2));
    REQUIRE_THAT(vga2_to_vga3(test2), GAEquals(3*e1 + 2*e2));
    REQUIRE_THAT(vga2_to_vga3(test3), GAEquals(2*e12));
    REQUIRE_THAT(vga2_to_vga3(test4), GAEquals(1 + 2*e12));
}

TEST_CASE("vga3_to_vga2", "[ga]") {
    auto test1 = vga3::Scalar(2);
    auto test2 = 3*vga3::e1 + 2*vga3::e2 + 3*vga3::e3;
    auto test3 = 2*vga3::e12 + vga3::e13 + vga3::e23;
    using namespace vga2;
    REQUIRE_THAT(vga3_to_vga2(test1), GAEquals(2));
    REQUIRE_THAT(vga3_to_vga2(test2), GAEquals(3*e1 + 2*e2));
    REQUIRE_THAT(vga3_to_vga2(test3), GAEquals(2*e12));
}

TEST_CASE("pga2_to_pga3", "[ga]") {
    auto test1 = pga2::Scalar(2);
    auto test2 = 3*pga2::e1 + 2*pga2::e2 + pga2::e0;
    auto test3 = 3*pga2::e12 + 2*pga2::e01 + pga2::e02;
    auto test4 = test1 + test3;
    using namespace pga3;
    REQUIRE_THAT(pga2_to_pga3(test1), GAEquals(2));
    REQUIRE_THAT(pga2_to_pga3(test2), GAEquals(3*e1 + 2*e2 + e0));
    REQUIRE_THAT(pga2_to_pga3(test3), GAEquals(3*e12 + 2*e01 + e02));
    REQUIRE_THAT(pga2_to_pga3(test4), GAEquals(2 + 3*e12 + 2*e01 + e02));
    REQUIRE_THAT(pga2_to_pga3_flat(test2), GAEquals(3*e13 + 2*e23 + e03));
    REQUIRE_THAT(pga2_to_pga3_flat(test3), GAEquals(3*e123 + 2*e013 + e023));
}

TEST_CASE("pga3_to_pga2", "[ga]") {
    auto test1 = pga3::Scalar(2);
    auto test2 = 3*pga3::e1 + 2*pga3::e2 + 4*pga3::e3 + pga3::e0;
    auto test3 = (3*pga3::e1 + 2*pga3::e2 + pga3::e0) ^
                 (4*pga3::e2 - pga3::e3 + pga3::e0);
    auto test4 = test2.dual();
    using namespace pga2;
    REQUIRE_THAT(pga3_to_pga2(test1), GAEquals(2));
    REQUIRE_THAT(pga3_to_pga2(test2), GAEquals(3*e1 + 2*e2 + e0));
    REQUIRE_THAT(pga3_to_pga2(test3), GAEquals(12*e12 - 3*e01 + 2*e02));
    REQUIRE_THAT(pga3_to_pga2(test4), GAEquals((3*e1 + 2*e2 + e0).dual()));
}

TEST_CASE("vga2_to_pga2", "[ga]") {
    auto test1 = vga2::Scalar(2);
    auto test2 = 3*vga2::e1 + 2*vga2::e2;
    auto test3 = 1 + 2*vga2::e12;
    using namespace pga2;
    REQUIRE_THAT(vga2_to_pga2(test1), GAEquals(2));
    REQUIRE_THAT(vga2_to_pga2(test2), GAEquals((3*e1 + 2*e2 + e0).dual()));
    REQUIRE_THAT(vga2_to_pga2(test3), GAEquals(1 + 2*e12));
    REQUIRE_THAT(vga2_to_pga2_full(test2), GAEquals(-2*e1 + 3*e2));
}

TEST_CASE("pga2_to_vga2", "[ga]") {
    auto test1 = pga2::Scalar(2);
    auto test2 = (3*pga2::e1 + 2*pga2::e2 + 2*pga2::e0).dual();
    auto test3 = (2*pga2::e1 + 3*pga2::e2).dual();
    auto test4 = 3*pga2::e1 + 2*pga2::e2;
    using namespace vga2;
    REQUIRE_THAT(pga2_to_vga2(test1), GAEquals(2));
    REQUIRE_THAT(pga2_to_vga2(test2), GAEquals(1.5*e1 + e2));
    REQUIRE_THAT(pga2_to_vga2(test3), GAEquals(2*e1 + 3*e2));
    REQUIRE_THAT(pga2_to_vga2_cheat(test4), GAEquals(3*e1 + 2*e2));
}

TEST_CASE("vga3_to_pga3", "[ga]") {
    auto test1 = vga3::Scalar(2);
    auto test2 = 3*vga3::e1 + 2*vga3::e2 - vga3::e3;
    auto test3 = 3*vga3::e12 + 2*vga3::e23 + vga3::e13;
    auto test4 = test1 + test3;
    using namespace pga3;
    REQUIRE_THAT(vga3_to_pga3(test1), GAEquals(2));
    REQUIRE_THAT(vga3_to_pga3(test2), GAEquals((3*e1 + 2*e2 - e3 + e0).dual()));
    REQUIRE_THAT(vga3_to_pga3(test3), GAEquals(3*e3 + 2*e1 - e2));
    REQUIRE_THAT(vga3_to_pga3(test4), GAEquals(2 + 3*e12 + 2*e23 + e13));
    REQUIRE_THAT(vga3_to_pga3_full(test2), GAEquals(3*e23 - 2*e13 - e12));
}

TEST_CASE("pga3_to_vga3", "[ga]") {
    auto test1 = pga3::Scalar(2);
    auto test2 = (3*pga3::e1 + 2*pga3::e2 - pga3::e3 + 2*pga3::e0).dual();
    auto test3 = (3*pga3::e1 + 2*pga3::e2 - pga3::e3).dual();
    auto test4 = 2*pga3::e1 + 3*pga3::e2 - 4*pga3::e3;
    using namespace vga3;
    REQUIRE_THAT(pga3_to_vga3(test1), GAEquals(2));
    REQUIRE_THAT(pga3_to_vga3(test2), GAEquals(1.5*e1 + e2 - 0.5*e3));
    REQUIRE_THAT(pga3_to_vga3(test3), GAEquals(3*e1 + 2*e2 - e3));
    REQUIRE_THAT(pga3_to_vga3_cheat(test4), GAEquals(2*e1 + 3*e2 - 4*e3));
}

TEST_CASE("vga2_to_pga3", "[ga]") {
    auto test1 = vga2::Scalar(2);
    auto test2 = 3*vga2::e1 + 2*vga2::e2;
    auto test3 = 1 + 2*vga2::e12;
    using namespace pga3;
    REQUIRE_THAT(vga2_to_pga3(test1), GAEquals(2));
    REQUIRE_THAT(vga2_to_pga3(test2), GAEquals((3*e1 + 2*e2 + e0).dual()));
    REQUIRE_THAT(vga2_to_pga3(test3), GAEquals(1 + 2*e12));
    REQUIRE_THAT(vga2_to_pga3_full(test2), GAEquals(3*e23 - 2*e13));
}

TEST_CASE("pga3_to_vga2", "[ga]") {
    auto test1 = pga3::Scalar(2);
    auto test2 = (3*pga3::e1 + 2*pga3::e2 + pga3::e3 + 0.5*pga3::e0).dual();
    using namespace vga2;
    REQUIRE_THAT(pga3_to_vga2(test1), GAEquals(2));
    REQUIRE_THAT(pga3_to_vga2(test2), GAEquals(6*e1 + 4*e2));
}
