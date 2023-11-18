#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"

#include "ganim/ga/sga.hpp"

using namespace ganim;

TEST_CASE("BasisBlade multiplication", "[ga]") {
    constexpr auto one = 0UL;
    constexpr auto e1 = 1UL;
    constexpr auto e2 = 2UL;
    constexpr auto e3 = 4UL;
    constexpr auto e12 = 3UL;
    constexpr auto e13 = 5UL;
    constexpr auto e23 = 6UL;
    constexpr auto e123 = 7UL;
    constexpr auto vga = std::array<std::int8_t, 3>{1, 1, 1};
    constexpr auto mixed = std::array<std::int8_t, 3>{1, -1, 0};

    REQUIRE(basis_blade_product_sign<double, one, e1>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, one, e2>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, one, e12>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e1, one>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, one>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e1, e1>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, e2>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e3, e3>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e12, e12>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e123, e123>(vga) == -1);

    REQUIRE(basis_blade_product_sign<double, e1, e2>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, e1>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e13, e12>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e12, e23>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e23, e13>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e12, e13>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e23, e12>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e13, e23>(vga) == -1);

    REQUIRE(basis_blade_product_sign<double, e1, e123>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, e123>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e3, e123>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e123, e1>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e123, e2>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e123, e3>(vga) == 1);
    REQUIRE(basis_blade_product_sign<double, e12, e123>(vga) == -1);
    REQUIRE(basis_blade_product_sign<double, e123, e12>(vga) == -1);

    REQUIRE(basis_blade_product_sign<double, one, e1>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, one, e2>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, one, e12>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e1, one>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, one>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e1, e1>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, e2>(mixed) == -1);
    REQUIRE(basis_blade_product_sign<double, e3, e3>(mixed) == 0);
    REQUIRE(basis_blade_product_sign<double, e12, e12>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e123, e123>(mixed) == 0);

    REQUIRE(basis_blade_product_sign<double, e1, e2>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, e1>(mixed) == -1);
    REQUIRE(basis_blade_product_sign<double, e13, e12>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e12, e23>(mixed) == -1);
    REQUIRE(basis_blade_product_sign<double, e23, e13>(mixed) == 0);
    REQUIRE(basis_blade_product_sign<double, e12, e13>(mixed) == -1);
    REQUIRE(basis_blade_product_sign<double, e23, e12>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e13, e23>(mixed) == 0);

    REQUIRE(basis_blade_product_sign<double, e1, e123>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e2, e123>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e3, e123>(mixed) == 0);
    REQUIRE(basis_blade_product_sign<double, e123, e1>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e123, e2>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e123, e3>(mixed) == 0);
    REQUIRE(basis_blade_product_sign<double, e12, e123>(mixed) == 1);
    REQUIRE(basis_blade_product_sign<double, e123, e12>(mixed) == 1);
}

TEST_CASE("Multivector basics", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};

    const auto test = Multivector<double, metric, 1, 3>(2, 3);

    REQUIRE(test.binary_blade_project<0>() == 0);
    REQUIRE(test.binary_blade_project<1>() == 2);
    REQUIRE(test.binary_blade_project<2>() == 0);
    REQUIRE(test.binary_blade_project<3>() == 3);
    REQUIRE(test.binary_blade_project<4>() == 0);
}

TEST_CASE("Multivector Equality", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto test1 = Multivector<double, metric, 1, 2>(1, 0);
    const auto test2 = Multivector<double, metric, 1, 2>(0, 1);
    const auto test3 = Multivector<double, metric>();
    REQUIRE(test1 == test1);
    REQUIRE(test2 == test2);
    REQUIRE(test1 != test2);
    REQUIRE(test3 == test3);

    REQUIRE(!(test1 != test1));
    REQUIRE(!(test2 != test2));
    REQUIRE(!(test1 == test2));
    REQUIRE(!(test3 != test3));
}

TEST_CASE("Multivector grade project", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e12 = Multivector<double, metric, 3>(1);

    const auto test = 1 + 2*e1 + 3*e2 + 4*e12;
    const auto test0 = test.grade_project<0>();
    const auto test1 = test.grade_project<1>();
    const auto test2 = test.grade_project<2>();
    const auto test3 = test.grade_project<3>();
    static_assert(std::is_same_v<
        decltype(test3),
        const Multivector<double, metric>
    >);

    REQUIRE_THAT(test0, GAEquals(1));
    REQUIRE_THAT(test1, GAEquals(2*e1 + 3*e2));
    REQUIRE_THAT(test2, GAEquals(4*e12));
}

TEST_CASE("Multivector addition 1", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};

    auto test1 = Multivector<double, metric, 1, 3>(2, 3);
    const auto test2 = Multivector<double, metric, 1>(2);
    test1 += test2;
    REQUIRE(test1.binary_blade_project<1>() == 4);
    REQUIRE(test1.binary_blade_project<3>() == 3);
}

TEST_CASE("Multivector addition 2", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};

    const auto test1 = Multivector<double, metric, 1>(2);
    const auto test2 = Multivector<double, metric, 2>(3);
    const auto test3 = Multivector<double, metric, 1, 2>(4, 5);
    const auto test4 = test1 + test2;
    const auto test5 = test1 + test3;
    const auto test6 = test3 + test1;
    static_assert(std::is_same_v<
        decltype(test4),
        const Multivector<double, metric, 1, 2>
    >);
    static_assert(std::is_same_v<
        decltype(test5),
        const Multivector<double, metric, 1, 2>
    >);
    static_assert(std::is_same_v<
        decltype(test6),
        const Multivector<double, metric, 1, 2>
    >);
    REQUIRE(test4.binary_blade_project<0>() == 0);
    REQUIRE(test4.binary_blade_project<1>() == 2);
    REQUIRE(test4.binary_blade_project<2>() == 3);
    REQUIRE(test4.binary_blade_project<3>() == 0);

    REQUIRE(test5.binary_blade_project<0>() == 0);
    REQUIRE(test5.binary_blade_project<1>() == 6);
    REQUIRE(test5.binary_blade_project<2>() == 5);
    REQUIRE(test5.binary_blade_project<3>() == 0);

    REQUIRE(test6.binary_blade_project<0>() == 0);
    REQUIRE(test6.binary_blade_project<1>() == 6);
    REQUIRE(test6.binary_blade_project<2>() == 5);
    REQUIRE(test6.binary_blade_project<3>() == 0);
}

TEST_CASE("Multivector Scalar Multiplication", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};

    auto test1 = Multivector<double, metric, 1, 3>(1, 3);
    const auto test2 = 2 * test1;
    const auto test3 = test1 * 2;
    test1 *= 2;
    REQUIRE(test1.binary_blade_project<0>() == 0);
    REQUIRE(test1.binary_blade_project<1>() == 2);
    REQUIRE(test1.binary_blade_project<2>() == 0);
    REQUIRE(test1.binary_blade_project<3>() == 6);

    REQUIRE(test2.binary_blade_project<0>() == 0);
    REQUIRE(test2.binary_blade_project<1>() == 2);
    REQUIRE(test2.binary_blade_project<2>() == 0);
    REQUIRE(test2.binary_blade_project<3>() == 6);

    REQUIRE(test3.binary_blade_project<0>() == 0);
    REQUIRE(test3.binary_blade_project<1>() == 2);
    REQUIRE(test3.binary_blade_project<2>() == 0);
    REQUIRE(test3.binary_blade_project<3>() == 6);
}

TEST_CASE("Multivector Scalar Division", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    auto test1 = 2*e1 + 3*e2;
    auto test2 = test1 / 2;
    test1 /= 4;
    REQUIRE_THAT(test1, GAEquals(0.5*e1 + 0.75*e2));
    REQUIRE_THAT(test2, GAEquals(1*e1 + 1.5*e2));
}

TEST_CASE("Multivector subtraction", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto test1 = 2*e1;
    const auto test2 = 3*e2;
    const auto test3 = 4*e1 + 5*e2;
    const auto test4 = -test3;
    const auto test5 = test1 - test2;
    const auto test6 = test1 - test3;
    const auto test7 = test3 - test1;
    REQUIRE_THAT(test4, GAEquals(-4*e1 + -5*e2));
    REQUIRE_THAT(test5, GAEquals(2*e1 + -3*e2));
    REQUIRE_THAT(test6, GAEquals(-2*e1 + -5*e2));
    REQUIRE_THAT(test7, GAEquals(2*e1 + 5*e2));
}

TEST_CASE("Multivector multiplication", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e12 = Multivector<double, metric, 3>(1);

    const auto test1 = 2*e1;
    const auto test2 = 3*e2;
    const auto test3 = 4*e1 + 5*e2;
    const auto test4 = 6*e1 + 7*e2;
    const auto test5 = test1 * test2;
    const auto test6 = test3 * test5;
    const auto test7 = test3 * test4;
    REQUIRE_THAT(test5, GAEquals(6*e12));
    REQUIRE_THAT(test6, GAEquals(30*e1 + 24*e2));
    REQUIRE_THAT(test7, GAEquals(-11 - 2*e12));
}

TEST_CASE("Multivector Scalar Addition", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e12 = Multivector<double, metric, 3>(1);

    auto test1 = 1 + 4*e12;
    const auto test2 = 2 + test1;
    const auto test3 = test1 + 2;
    test1 += 2;
    REQUIRE_THAT(test1, GAEquals(3 + 4*e12));
    REQUIRE_THAT(test2, GAEquals(3 + 4*e12));
    REQUIRE_THAT(test3, GAEquals(3 + 4*e12));
}

TEST_CASE("Multivector Outer Product", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e12 = Multivector<double, metric, 3>(1);

    const auto test1 = 2*e1;
    const auto test2 = 3*e2;
    const auto test3 = 4*e1 + 5*e2;
    const auto test4 = 6*e1 + 7*e2;
    const auto test5 = test1 ^ test2;
    const auto test6 = test3 ^ test5;
    const auto test7 = test3 ^ test4;
    static_assert(std::is_same_v<
        decltype(test6),
        const Multivector<double, metric>
    >);
    REQUIRE_THAT(test5, GAEquals(6*e12));
    REQUIRE_THAT(test7, GAEquals(-2*e12));
}

TEST_CASE("Multivector Inner Product", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);

    const auto test1 = 2*e1;
    const auto test2 = 3*e2;
    const auto test3 = 4*e1 + 5*e2;
    const auto test4 = 6*e1 + 7*e2;
    const auto test5 = test1 | test2;
    const auto test6 = test3 | test5;
    const auto test7 = test3 | test4;
    static_assert(std::is_same_v<
        decltype(test5),
        const Multivector<double, metric>
    >);
    static_assert(std::is_same_v<
        decltype(test6),
        const Multivector<double, metric>
    >);
    REQUIRE_THAT(test7, GAEquals(-11));
}

TEST_CASE("Multivector Contractions", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};
    const auto e = Multivector<double, metric, 0>(1);
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e12 = Multivector<double, metric, 3>(1);

    const auto test1 = 2*e;
    const auto test2 = 3*e1 + 4*e2;
    const auto test3 = 5*e12;
    const auto test4 = test1 << test2;
    const auto test5 = test2 << test3;
    const auto test6 = test2 >> test1;
    const auto test7 = test3 >> test2;
    const auto test8 = test2 << test1;
    const auto test9 = test3 << test2;
    const auto test10 = test1 >> test2;
    const auto test11 = test2 >> test3;
    static_assert(std::is_same_v<
        decltype(test8),
        const Multivector<double, metric>
    >);
    static_assert(std::is_same_v<
        decltype(test9),
        const Multivector<double, metric>
    >);
    static_assert(std::is_same_v<
        decltype(test10),
        const Multivector<double, metric>
    >);
    static_assert(std::is_same_v<
        decltype(test11),
        const Multivector<double, metric>
    >);
    REQUIRE_THAT(test4, GAEquals(6*e1 + 8*e2));
    REQUIRE_THAT(test5, GAEquals(20*e1 + 15*e2));
    REQUIRE_THAT(test6, GAEquals(6*e1 + 8*e2));
    REQUIRE_THAT(test7, GAEquals(-20*e1 - 15*e2));
}

TEST_CASE("Multivector dual", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 3>{1, -1, 0};
    const auto e = Multivector<double, metric, 0>(1);
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e3 = Multivector<double, metric, 4>(1);
    const auto e12 = Multivector<double, metric, 3>(1);
    const auto e13 = Multivector<double, metric, 5>(1);
    const auto e23 = Multivector<double, metric, 6>(1);
    const auto e123 = Multivector<double, metric, 7>(1);

    REQUIRE_THAT(e1.dual(), GAEquals(!e1));
    REQUIRE_THAT(e2.dual(), GAEquals(!e2));

    REQUIRE_THAT((!e), GAEquals(e123));
    REQUIRE_THAT((!e1), GAEquals(e23));
    REQUIRE_THAT((!e2), GAEquals(-e13));
    REQUIRE_THAT((!e3), GAEquals(e12));
    REQUIRE_THAT((!e12), GAEquals(e3));
    REQUIRE_THAT((!e13), GAEquals(-e2));
    REQUIRE_THAT((!e23), GAEquals(e1));
    REQUIRE_THAT((!e123), GAEquals(e));

    REQUIRE_THAT(e.dual().undual(), GAEquals(e));
    REQUIRE_THAT(e1.dual().undual(), GAEquals(e1));
    REQUIRE_THAT(e2.dual().undual(), GAEquals(e2));
    REQUIRE_THAT(e3.dual().undual(), GAEquals(e3));
    REQUIRE_THAT(e12.dual().undual(), GAEquals(e12));
    REQUIRE_THAT(e13.dual().undual(), GAEquals(e13));
    REQUIRE_THAT(e23.dual().undual(), GAEquals(e23));
    REQUIRE_THAT(e123.dual().undual(), GAEquals(e123));

    REQUIRE_THAT((e1 + e2 + e3).dual(), GAEquals(e12 + e23 - e13));
}

TEST_CASE("Multivector regressive product", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 3>{1, -1, 0};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e3 = Multivector<double, metric, 4>(1);
    const auto e13 = Multivector<double, metric, 5>(1);
    const auto e23 = Multivector<double, metric, 6>(1);

    REQUIRE_THAT((e1 + e3) & e2, GAEquals(0));
    REQUIRE_THAT(e13 & e23, GAEquals(e3));
}

TEST_CASE("Multivector involutions", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 4>{1, 1, 1, 1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e3 = Multivector<double, metric, 4>(1);
    const auto e4 = Multivector<double, metric, 4>(1);
    const auto test = 1 + e1 + e2 + (e1^e2) + (e2^e3^e4) + (e1^e2^e3^e4);
    const auto test2 = ~test;
    const auto test3 = test.involute();
    REQUIRE_THAT(test2,
            GAEquals(1 + e1 + e2 - (e1^e2) - (e2^e3^e4) + (e1^e2^e3^e4)));
    REQUIRE_THAT(test3,
            GAEquals(1 - e1 - e2 + (e1^e2) - (e2^e3^e4) + (e1^e2^e3^e4)));
}

TEST_CASE("Multivector norm", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 3>{1, -1, 0};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e3 = Multivector<double, metric, 4>(1);
    const auto test1 = 2 + e1 + e2 + e3;
    const auto test2 = 1 + (e1^e2) + (e2^e3);
    REQUIRE(test1.norm2() == 4);
    REQUIRE(test1.norm() == 2);
    REQUIRE_THAT(test1.normalized(), GAEquals(1 + 0.5*e1 + 0.5*e2 + 0.5*e3));
    REQUIRE(test2.norm2() == 0);
    REQUIRE(test2.norm() == 0);
}

TEST_CASE("Multivector component blade projection", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 3>{1, -1, 0};
    constexpr auto e1 = Multivector<double, metric, 1, 2, 4>(1, 0, 0);
    constexpr auto e2 = Multivector<double, metric, 1, 2, 4>(0, 1, 0);
    constexpr auto e3 = Multivector<double, metric, 1, 2, 4>(0, 0, 1);
    constexpr auto e12 = Multivector<double, metric, 3, 5, 6>(1, 0, 0);
    constexpr auto e21 = -e12;
    auto test = 2*e1 + 3*e2 + 4*e3 + 5*e12;
    REQUIRE(test.blade_project<e1>() == 2);
    REQUIRE(test.blade_project<e2>() == 3);
    REQUIRE(test.blade_project<e3>() == 4);
    REQUIRE(test.blade_project<e12>() == 5);
    REQUIRE(test.blade_project<e21>() == -5);
    REQUIRE(test.blade_project<2*e2>() == 1.5);
}

TEST_CASE("Multivector empty products", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 1>{1};
    constexpr auto e1 = Multivector<double, metric, 1>(1);
    constexpr auto z = Multivector<double, metric>();
    REQUIRE_THAT(e1 * z, GAEquals(z));
    REQUIRE_THAT(z * e1, GAEquals(z));
}

TEST_CASE("Multivector constructing from parts", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    Multivector<double, metric, 0, 1, 2> test = e1 + e2;
    REQUIRE(test.binary_blade_project<0>() == 0);
    REQUIRE(test.binary_blade_project<1>() == 1);
    REQUIRE(test.binary_blade_project<2>() == 1);
    test = 2*e1;
    REQUIRE(test.binary_blade_project<0>() == 0);
    REQUIRE(test.binary_blade_project<1>() == 2);
    REQUIRE(test.binary_blade_project<2>() == 0);
    test = 3*e2;
    REQUIRE(test.binary_blade_project<0>() == 0);
    REQUIRE(test.binary_blade_project<1>() == 0);
    REQUIRE(test.binary_blade_project<2>() == 3);

    Multivector<double, metric, 0, 1> test2 = 1;
    REQUIRE(test2.binary_blade_project<0>() == 1);
    REQUIRE(test2.binary_blade_project<1>() == 0);
    test2 = {1, 1};
    test2 = 2;
    REQUIRE(test2.binary_blade_project<0>() == 2);
    REQUIRE(test2.binary_blade_project<1>() == 0);
}

TEST_CASE("Multivector different comparisons", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    REQUIRE_THAT(e1, GAEquals(e1 + e2 - e2));
    REQUIRE_THAT(e1, !GAEquals(e1 + e2));
}

TEST_CASE("Multivector scalar comparisons", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 1>{1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto test1 = 1 + e1;
    const auto test2 = 1 + e1 - e1;
    const auto test3 = 0 + e1;
    const auto test4 = 0 + e1 - e1;
    REQUIRE(test1 != 1);
    REQUIRE(1 != test1);
    REQUIRE(test2 == 1);
    REQUIRE(1 == test2);
    REQUIRE(test3 != 0);
    REQUIRE(0 != test3);
    REQUIRE(test4 == 0);
    REQUIRE(0 == test4);
}

TEST_CASE("Multivector assignment", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e = Multivector<double, metric, 0>(1);
    const auto e1 = Multivector<double, metric, 1>(1);
    auto test1 = e;
    auto test2 = e + e1;
    auto test3 = e1;
    auto test4 = Multivector<double, metric, 1, 2>(0);
    (void)test4;
    test1 = 0;
    test2 = 0;
    test3 = 0;
    REQUIRE_THAT(test1, GAEquals(0));
    REQUIRE_THAT(test2, GAEquals(0));
    REQUIRE_THAT(test3, GAEquals(0));
    test1 = 1;
    test2 = 1;
    REQUIRE_THAT(test1, GAEquals(1));
    REQUIRE_THAT(test2, GAEquals(1));
    test2 = e1;
    REQUIRE_THAT(test2, GAEquals(e1));
}
