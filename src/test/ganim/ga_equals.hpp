#ifndef GANIM_TEST_GA_EQUALS_HPP
#define GANIM_TEST_GA_EQUALS_HPP

#include "ganim/ga/vga2.hpp"
#include "ganim/ga/vga3.hpp"
#include "ganim/ga/pga2.hpp"
#include "ganim/ga/pga3.hpp"

#include <catch2/matchers/catch_matchers_templated.hpp>

// This file contains a matcher for testing if multivectors are equal to each
// other, with a possible epsilon value used for absolute error tolerance.  Just
// use REQUIRE_THAT(mv1, GAEquals(mv2)) to check that the two multivectors are
// equal, and REQUIRE_THAT(mv1, GAEquals(mv2, eps)) to use an epsilon value.

template <
    ganim::scalar Scalar,
    auto metric,
    std::uint64_t... bases1,
    std::uint64_t... bases2
>
constexpr bool compare_mvs(
    const ganim::Multivector<Scalar, metric, bases1...>& mv1,
    const ganim::Multivector<Scalar, metric, bases2...>& mv2,
    [[maybe_unused]] double eps
)
{
    return ((std::abs(
                mv1.template binary_blade_project<bases1>() -
                mv2.template binary_blade_project<bases1>()) < eps) and ...)
        and
           ((std::abs(
                mv1.template binary_blade_project<bases2>() -
                mv2.template binary_blade_project<bases2>()) < eps) and ...);
}

template <
    ganim::scalar Scalar,
    auto metric,
    std::uint64_t... bases
>
constexpr bool compare_mv_scalar(
    const ganim::Multivector<Scalar, metric, bases...>& mv1,
    double scalar,
    double eps
)
{
    return std::abs(mv1.template binary_blade_project<0>() - scalar) < eps and
        ((std::abs(mv1.template binary_blade_project<bases>()) < eps
              or bases == 0) and ...);
}

template <typename T>
struct GAEquals : public Catch::Matchers::MatcherGenericBase {
    GAEquals(const T& mv, double eps = 0) : mv(mv), eps(eps) {}

    template <typename U>
    bool match(const U& other) const
    {
        str << "  ";
        write_mv(other);
        if (eps > 0) {
            if constexpr (requires{mv.template binary_blade_project<0>();}) {
                return compare_mvs(other, mv, eps);
            }
            else {
                return compare_mv_scalar(other, mv, eps);
            }
        }
        else return mv == other;
    }
    virtual std::string describe() const override
    {
        str << " == ";
        write_mv(mv);
        return str.str();
    }
    static constexpr const char names[] = "123456789";
    template <typename U>
    void write_mv(const U& v) const
    {
        if constexpr (requires{str << v;}) {
            str << v;
        }
        else {
            output_multivector<names>(str, v);
        }
    }

    T mv;
    // Yes I know this is terrible, but Catch2 refuses to learn how to output
    // multivectors no matter how hard I try
    mutable std::stringstream str;
    double eps;
};

#endif
