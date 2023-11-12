#include <catch2/catch_test_macros.hpp>

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

    REQUIRE(test.blade_project<0>() == 0);
    REQUIRE(test.blade_project<1>() == 2);
    REQUIRE(test.blade_project<2>() == 0);
    REQUIRE(test.blade_project<3>() == 3);
    REQUIRE(test.blade_project<4>() == 0);
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

    REQUIRE(test0 == 1);
    REQUIRE(test1 == 2*e1 + 3*e2);
    REQUIRE(test2 == 4*e12);
}

TEST_CASE("Multivector addition 1", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, -1};

    auto test1 = Multivector<double, metric, 1, 3>(2, 3);
    const auto test2 = Multivector<double, metric, 1>(2);
    test1 += test2;
    REQUIRE(test1.blade_project<1>() == 4);
    REQUIRE(test1.blade_project<3>() == 3);
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
    REQUIRE(test4.blade_project<0>() == 0);
    REQUIRE(test4.blade_project<1>() == 2);
    REQUIRE(test4.blade_project<2>() == 3);
    REQUIRE(test4.blade_project<3>() == 0);

    REQUIRE(test5.blade_project<0>() == 0);
    REQUIRE(test5.blade_project<1>() == 6);
    REQUIRE(test5.blade_project<2>() == 5);
    REQUIRE(test5.blade_project<3>() == 0);

    REQUIRE(test6.blade_project<0>() == 0);
    REQUIRE(test6.blade_project<1>() == 6);
    REQUIRE(test6.blade_project<2>() == 5);
    REQUIRE(test6.blade_project<3>() == 0);
}

TEST_CASE("Multivector Scalar Multiplication", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};

    auto test1 = Multivector<double, metric, 1, 3>(1, 3);
    const auto test2 = 2 * test1;
    const auto test3 = test1 * 2;
    test1 *= 2;
    REQUIRE(test1.blade_project<0>() == 0);
    REQUIRE(test1.blade_project<1>() == 2);
    REQUIRE(test1.blade_project<2>() == 0);
    REQUIRE(test1.blade_project<3>() == 6);

    REQUIRE(test2.blade_project<0>() == 0);
    REQUIRE(test2.blade_project<1>() == 2);
    REQUIRE(test2.blade_project<2>() == 0);
    REQUIRE(test2.blade_project<3>() == 6);

    REQUIRE(test3.blade_project<0>() == 0);
    REQUIRE(test3.blade_project<1>() == 2);
    REQUIRE(test3.blade_project<2>() == 0);
    REQUIRE(test3.blade_project<3>() == 6);
}

TEST_CASE("Multivector Scalar Division", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    auto test1 = 2*e1 + 3*e2;
    auto test2 = test1 / 2;
    test1 /= 4;
    REQUIRE(test1 == 0.5*e1 + 0.75*e2);
    REQUIRE(test2 == 1*e1 + 1.5*e2);
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
    REQUIRE(test4 == -4*e1 + -5*e2);
    REQUIRE(test5 == 2*e1 + -3*e2);
    REQUIRE(test6 == -2*e1 + -5*e2);
    REQUIRE(test7 == 2*e1 + 5*e2);
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
    REQUIRE(test5 == 6*e12);

    REQUIRE(test6 == 30*e1 + 24*e2);

    REQUIRE(test7.blade_project<0>() == -11);
    REQUIRE(test7 == -11 - 2*e12);
}

TEST_CASE("Multivector Scalar Addition", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 2>{1, 1};
    const auto e12 = Multivector<double, metric, 3>(1);

    auto test1 = 1 + 4*e12;
    const auto test2 = 2 + test1;
    const auto test3 = test1 + 2;
    test1 += 2;
    REQUIRE(test1 == 3 + 4*e12);
    REQUIRE(test2 == 3 + 4*e12);
    REQUIRE(test3 == 3 + 4*e12);
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
    REQUIRE(test5 == 6*e12);
    REQUIRE(test7 == -2*e12);
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
    REQUIRE(test7 == -11);
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
    REQUIRE(test4 == 6*e1 + 8*e2);
    REQUIRE(test5 == 20*e1 + 15*e2);
    REQUIRE(test6 == 6*e1 + 8*e2);
    REQUIRE(test7 == -20*e1 - 15*e2);
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

    REQUIRE(e1.dual() == !e1);
    REQUIRE(e2.dual() == !e2);

    REQUIRE((!e) == e123);
    REQUIRE((!e1) == e23);
    REQUIRE((!e2) == -e13);
    REQUIRE((!e3) == e12);
    REQUIRE((!e12) == e3);
    REQUIRE((!e13) == -e2);
    REQUIRE((!e23) == e1);
    REQUIRE((!e123) == e);

    REQUIRE(e.dual().undual() == e);
    REQUIRE(e1.dual().undual() == e1);
    REQUIRE(e2.dual().undual() == e2);
    REQUIRE(e3.dual().undual() == e3);
    REQUIRE(e12.dual().undual() == e12);
    REQUIRE(e13.dual().undual() == e13);
    REQUIRE(e23.dual().undual() == e23);
    REQUIRE(e123.dual().undual() == e123);

    REQUIRE((e1 + e2 + e3).dual() == e12 + e23 - e13);
}

TEST_CASE("Multivector regressive product", "[ga]") {
    constexpr auto metric = std::array<std::int8_t, 3>{1, -1, 0};
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e2 = Multivector<double, metric, 2>(1);
    const auto e3 = Multivector<double, metric, 4>(1);
    const auto e13 = Multivector<double, metric, 5>(1);
    const auto e23 = Multivector<double, metric, 6>(1);

    REQUIRE(((e1 + e3) & e2) == 0);
    REQUIRE((e13 & e23) == e3);
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
    REQUIRE(test2 == 1 + e1 + e2 - (e1^e2) - (e2^e3^e4) + (e1^e2^e3^e4));
    REQUIRE(test3 == 1 - e1 - e2 + (e1^e2) - (e2^e3^e4) + (e1^e2^e3^e4));
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
    REQUIRE(test1.normalized() == 1 + 0.5*e1 + 0.5*e2 + 0.5*e3);
    REQUIRE(test2.norm2() == 0);
    REQUIRE(test2.norm() == 0);
}
