#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "test/ganim/ga_equals.hpp"

#include <cmath>

#include "ganim/ga/exp.hpp"

#include "ganim/math.hpp"

using namespace ganim;

static_assert(always_simple<pga3::Scalar>);
static_assert(always_simple<pga3::Vec>);
static_assert(!always_simple<pga3::Bivec>);
static_assert(always_simple<pga3::Trivec>);
static_assert(always_simple<pga3::Quadvec>);

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
    REQUIRE_THAT(test3, GAEquals(1, 1e-5));
    REQUIRE_THAT(test4, GAEquals(1, 1e-5));
}

TEST_CASE("ga_inv 3D PGA", "[ga]") {
    using namespace pga3;
    const auto b = e12 + 2*e23 - e01 + 5*e02;
    const auto test = b * ga_inv(b);
    REQUIRE_THAT(test, GAEquals(1, 1e-5));
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
    REQUIRE_THAT(res1, GAEquals(std::cosh(1) + std::sinh(1)*e12, 1e-5));
    REQUIRE_THAT(res2, GAEquals(std::cosh(2) + std::sinh(2)*e12, 1e-5));
    REQUIRE_THAT(res3, GAEquals(
                std::cosh(2) + std::sinh(2)*e12 + std::sinh(2)*e13/2, 1e-5));
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
    REQUIRE_THAT(res1, GAEquals(std::cos(1) + std::sin(1)*e12, 1e-5));
    REQUIRE_THAT(res2, GAEquals(std::cos(2) + std::sin(2)*e12, 1e-5));
    REQUIRE_THAT(res3, GAEquals(
                std::cos(2) + std::sin(2)*e12 + std::sin(2)*e13/2, 1e-5));
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
    REQUIRE_THAT(res1, GAEquals(1 + e12));
    REQUIRE_THAT(res2, GAEquals(1 + 2*e12));
    REQUIRE_THAT(res3, GAEquals(1 + 2*e12 + e13));
}

TEST_CASE("ga_exp 3D PGA", "[ga]") {
    using namespace ganim::pga3;
    auto res1 = ga_exp(e12);
    auto res2 = ga_exp(2*e12);
    auto res3 = ga_exp(2*e12 + e01);
    auto res4 = ga_exp(e01);
    auto res5 = ga_exp(e12 + e03);
    REQUIRE_THAT(res1, GAEquals(std::cos(1) + std::sin(1)*e12, 1e-5));
    REQUIRE_THAT(res2, GAEquals(std::cos(2) + std::sin(2)*e12, 1e-5));
    REQUIRE_THAT(res3, GAEquals(
                std::cos(2) + std::sin(2)*e12 + std::sin(2)*e01/2, 1e-5));
    REQUIRE_THAT(res4, GAEquals(1 + e01));
    // exp(e12) exp(e03)
    // = (cos(1) + e12 sin(1)) (1 + e03)
    // = cos(1) + cos(1) e03 + sin(1) e12 + sin(1) e0123
    REQUIRE_THAT(res5, GAEquals(
        std::cos(1) + std::cos(1)*e03 + std::sin(1)*e12 + std::sin(1)*e0123,
        1e-5));
}

TEST_CASE("simple_log positive", "[ga]")
{
    constexpr auto metric = std::array<int8_t, 3>{1, -1, 0};
    constexpr auto e1 = Multivector<double, metric, 1>(1);
    constexpr auto e2 = Multivector<double, metric, 2>(1);
    constexpr auto e12 = Multivector<double, metric, 3>(1);
    constexpr auto e13 = Multivector<double, metric, 5>(1);
    const auto test1 = simple_log(simple_exp(e12));
    const auto test2 = simple_log(simple_exp(2*e12));
    const auto test3 = simple_log(simple_exp(2*e12 + e13));
    const auto r1 = ((2*e1 + e2) * (3*e1 + e2)).normalized();
    const auto test4 = simple_exp(simple_log(r1));
    REQUIRE_THAT(test1, GAEquals(e12, 1e-5));
    REQUIRE_THAT(test2, GAEquals(2*e12, 1e-5));
    REQUIRE_THAT(test3, GAEquals(2*e12 + e13, 1e-5));
    REQUIRE_THAT(test4, GAEquals(r1, 1e-5));
}

TEST_CASE("simple_log negative", "[ga]")
{
    constexpr auto metric = std::array<int8_t, 3>{1, 1, 0};
    constexpr auto e1 = Multivector<double, metric, 1>(1);
    constexpr auto e2 = Multivector<double, metric, 2>(1);
    constexpr auto e12 = Multivector<double, metric, 3>(1);
    constexpr auto e13 = Multivector<double, metric, 5>(1);
    const auto test1 = simple_log(simple_exp(e12));
    const auto test2 = simple_log(simple_exp(2*e12));
    const auto test3 = simple_log(simple_exp(2*e12 + e13));
    const auto r1 = e12;
    const auto r2 = ((2*e1 + e2) * (e1 + 2*e2)).normalized();
    const auto test4 = simple_exp(simple_log(r1));
    const auto test5 = simple_exp(simple_log(r2));
    REQUIRE_THAT(test1, GAEquals(e12, 1e-5));
    REQUIRE_THAT(test2, GAEquals(2*e12, 1e-5));
    REQUIRE_THAT(test3, GAEquals(2*e12 + e13, 1e-5));
    REQUIRE_THAT(test4, GAEquals(r1, 1e-5));
    REQUIRE_THAT(test5, GAEquals(r2, 1e-5));
}

TEST_CASE("simple_log zero", "[ga]")
{
    constexpr auto metric = std::array<int8_t, 3>{0, 0, 0};
    constexpr auto e12 = Multivector<double, metric, 3>(1);
    constexpr auto e13 = Multivector<double, metric, 5>(1);
    const auto test1 = simple_log(simple_exp(e12));
    const auto test2 = simple_log(simple_exp(2*e12));
    const auto test3 = simple_log(simple_exp(2*e12 + e13));
    const auto r1 = 1 + e12;
    const auto test4 = simple_exp(simple_log(r1));
    REQUIRE_THAT(test1, GAEquals(e12));
    REQUIRE_THAT(test2, GAEquals(2*e12));
    REQUIRE_THAT(test3, GAEquals(2*e12 + e13));
    REQUIRE_THAT(test4, GAEquals(r1));
}

TEST_CASE("ga_log 3D PGA", "[ga]") {
    using namespace ganim::pga3;
    auto res1 = ga_log(ga_exp(e12));
    auto res2 = ga_log(ga_exp(2*e12));
    auto res3 = ga_log(ga_exp(2*e12 + e01));
    auto res4 = ga_log(ga_exp(e01));
    auto res5 = ga_log(ga_exp(e12 + e03));
    auto res6 = ga_exp(ga_log(1 + e01));
    auto r = ((e1 + e0)*(e2 + e3 + e0)*(2*e2 - e0)*(3*e1 - e3)).normalized();
    auto res7 = ga_exp(ga_log(r));
    REQUIRE_THAT(res1, GAEquals(e12, 1e-5));
    REQUIRE_THAT(res2, GAEquals(2*e12, 1e-5));
    REQUIRE_THAT(res3, GAEquals(2*e12 + e01, 1e-5));
    REQUIRE_THAT(res4, GAEquals(e01, 1e-5));
    REQUIRE_THAT(res5, GAEquals(e12 + e03, 1e-5));
    REQUIRE_THAT(res6, GAEquals(1 + e01, 1e-5));
    REQUIRE_THAT(res7, GAEquals(r, 1e-5));
}

TEST_CASE("ga_log negative translation", "[ga]") {
    using namespace ganim::pga3;
    auto res1 = ga_exp(ga_log(-1 - e01));
    auto r = (3*e1 + 2*e2 + e3 + e0).dual() * (-e1 - e2 - 4*e3 + e0).dual();
    auto res2 = ga_exp(ga_log(r));
    auto res3 = ga_exp(ga_log(Even(-1)));
    REQUIRE_THAT(res1, GAEquals(-1 - e01, 1e-5));
    REQUIRE_THAT(res2, GAEquals(r, 1e-5));
    REQUIRE_THAT(res3, GAEquals(-1, 1e-5));
}
