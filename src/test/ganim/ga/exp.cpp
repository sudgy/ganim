#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>

#include "ganim/ga/exp.hpp"

#include "ganim/math.hpp"

using namespace ganim;

static_assert(always_simple<pga3::Scalar>);
static_assert(always_simple<pga3::Vector>);
static_assert(!always_simple<pga3::Bivector>);
static_assert(always_simple<pga3::Trivector>);
static_assert(always_simple<pga3::Quadvector>);

using namespace Catch::Matchers;

TEST_CASE("ga_inv simple", "[ga]") {
    constexpr auto metric = std::array<int8_t, 3>{1, -1, 0};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e3 = Multivector<double, metric, 4>(1);
    const auto test1 = e1 + 2*e2;
    const auto test2 = 3*(e1^e2) - (e2^e3);
    const auto test3 = test1 * ga_inv(test1);
    const auto test4 = test2 * ga_inv(test2);
    REQUIRE_THAT(test3.binary_blade_project<0>(), WithinAbs(1, 1e-5));
    REQUIRE_THAT(test3.binary_blade_project<1>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test3.binary_blade_project<2>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test3.binary_blade_project<4>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test4.binary_blade_project<0>(), WithinAbs(1, 1e-5));
    REQUIRE_THAT(test4.binary_blade_project<3>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test4.binary_blade_project<5>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test4.binary_blade_project<6>(), WithinAbs(0, 1e-5));
}

TEST_CASE("ga_inv 3D PGA", "[ga]") {
    using namespace pga3;
    const auto b = e12 + 2*e23 - e01 + 5*e02;
    const auto test = b * ga_inv(b);
    REQUIRE_THAT(test.blade_project<e>(), WithinAbs(1, 1e-5));
    REQUIRE_THAT(test.blade_project<e12>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test.blade_project<e13>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test.blade_project<e23>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test.blade_project<e01>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test.blade_project<e02>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test.blade_project<e03>(), WithinAbs(0, 1e-5));
    REQUIRE_THAT(test.blade_project<e0123>(), WithinAbs(0, 1e-5));
}

TEST_CASE("simple_exp positive", "[ga]") {
    constexpr auto metric = std::array<int8_t, 3>{1, -1, 0};
    const auto e12 = Multivector<double, metric, 3>(1);
    const auto e13 = Multivector<double, metric, 5>(1);
    const auto res1 = simple_exp(e12);
    const auto res2 = simple_exp(2*e12);
    const auto res3 = simple_exp(2*e12 + e13);
    static_assert(std::is_same_v<
        decltype(res1),
        const Multivector<double, metric, 0, 3>
    >);
    static_assert(std::is_same_v<
        decltype(res2),
        const Multivector<double, metric, 0, 3>
    >);
    static_assert(std::is_same_v<
        decltype(res3),
        const Multivector<double, metric, 0, 3, 5>
    >);
    REQUIRE_THAT(res1.binary_blade_project<0>(), WithinRel(std::cosh(1), 1e-5));
    REQUIRE_THAT(res1.binary_blade_project<3>(), WithinRel(std::sinh(1), 1e-5));
    REQUIRE_THAT(res2.binary_blade_project<0>(), WithinRel(std::cosh(2), 1e-5));
    REQUIRE_THAT(res2.binary_blade_project<3>(), WithinRel(std::sinh(2), 1e-5));
    REQUIRE_THAT(res3.binary_blade_project<0>(), WithinRel(std::cosh(2), 1e-5));
    REQUIRE_THAT(res3.binary_blade_project<3>(), WithinRel(std::sinh(2), 1e-5));
    REQUIRE_THAT(res3.binary_blade_project<5>(), WithinRel(std::sinh(2)/2, 1e-5));
}

TEST_CASE("simple_exp negative", "[ga]") {
    constexpr auto metric = std::array<int8_t, 3>{1, 1, 0};
    const auto e12 = Multivector<double, metric, 3>(1);
    const auto e13 = Multivector<double, metric, 5>(1);
    const auto res1 = simple_exp(e12);
    const auto res2 = simple_exp(2*e12);
    const auto res3 = simple_exp(2*e12 + e13);
    static_assert(std::is_same_v<
        decltype(res1),
        const Multivector<double, metric, 0, 3>
    >);
    static_assert(std::is_same_v<
        decltype(res2),
        const Multivector<double, metric, 0, 3>
    >);
    static_assert(std::is_same_v<
        decltype(res3),
        const Multivector<double, metric, 0, 3, 5>
    >);
    REQUIRE_THAT(res1.binary_blade_project<0>(), WithinRel(std::cos(1), 1e-5));
    REQUIRE_THAT(res1.binary_blade_project<3>(), WithinRel(std::sin(1), 1e-5));
    REQUIRE_THAT(res2.binary_blade_project<0>(), WithinRel(std::cos(2), 1e-5));
    REQUIRE_THAT(res2.binary_blade_project<3>(), WithinRel(std::sin(2), 1e-5));
    REQUIRE_THAT(res3.binary_blade_project<0>(), WithinRel(std::cos(2), 1e-5));
    REQUIRE_THAT(res3.binary_blade_project<3>(), WithinRel(std::sin(2), 1e-5));
    REQUIRE_THAT(res3.binary_blade_project<5>(), WithinRel(std::sin(2)/2, 1e-5));
}

TEST_CASE("simple_exp zero", "[ga]") {
    constexpr auto metric = std::array<int8_t, 3>{0, 0, 0};
    const auto e12 = Multivector<double, metric, 3>(1);
    const auto e13 = Multivector<double, metric, 5>(1);
    const auto res1 = simple_exp(e12);
    const auto res2 = simple_exp(2*e12);
    const auto res3 = simple_exp(2*e12 + e13);
    static_assert(std::is_same_v<
        decltype(res1),
        const Multivector<double, metric, 0, 3>
    >);
    static_assert(std::is_same_v<
        decltype(res2),
        const Multivector<double, metric, 0, 3>
    >);
    static_assert(std::is_same_v<
        decltype(res3),
        const Multivector<double, metric, 0, 3, 5>
    >);
    REQUIRE(res1 == 1 + e12);
    REQUIRE(res2 == 1 + 2*e12);
    REQUIRE(res3 == 1 + 2*e12 + e13);
}

TEST_CASE("ga_exp 3D PGA", "[ga]") {
    using namespace ganim::pga3;
    auto res1 = ga_exp(e12);
    auto res2 = ga_exp(2*e12);
    auto res3 = ga_exp(2*e12 + e01);
    auto res4 = ga_exp(e01);
    auto res5 = ga_exp(e12 + e03);
    REQUIRE_THAT(res1.blade_project<e>(), WithinRel(std::cos(1), 1e-5));
    REQUIRE_THAT(res1.blade_project<e12>(), WithinRel(std::sin(1), 1e-5));
    REQUIRE_THAT(res2.blade_project<e>(), WithinRel(std::cos(2), 1e-5));
    REQUIRE_THAT(res2.blade_project<e12>(), WithinRel(std::sin(2), 1e-5));
    REQUIRE_THAT(res3.blade_project<e>(), WithinRel(std::cos(2), 1e-5));
    REQUIRE_THAT(res3.blade_project<e12>(), WithinRel(std::sin(2), 1e-5));
    REQUIRE_THAT(res3.blade_project<e01>(), WithinRel(std::sin(2)/2, 1e-5));
    REQUIRE(res4 == 1 + e01 + 0*e0123);
    // exp(e12) exp(e03)
    // = (cos(1) + e12 sin(1)) (1 + e03)
    // = cos(1) + cos(1) e03 + sin(1) e12 + sin(1) e0123
    REQUIRE_THAT(res5.blade_project<e>(), WithinRel(std::cos(1), 1e-5));
    REQUIRE_THAT(res5.blade_project<e03>(), WithinRel(std::cos(1), 1e-5));
    REQUIRE_THAT(res5.blade_project<e12>(), WithinRel(std::sin(1), 1e-5));
    REQUIRE_THAT(res5.blade_project<e0123>(), WithinRel(std::sin(1), 1e-5));
}
