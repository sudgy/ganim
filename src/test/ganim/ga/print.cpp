#include <catch2/catch_test_macros.hpp>

#include "ganim/ga/print.hpp"
#include "ganim/ga/pga3.hpp"

using namespace ganim;

namespace {
    constexpr const char names[] = "135";
    constexpr auto metric = std::array<std::int8_t, 3>{1, -1, 0};
    template <std::uint64_t... bases>
    std::ostream& operator<<(
        std::ostream& stream,
        const Multivector<double, metric, bases...>& m
    )
    {
        output_multivector<names>(stream, m);
        return stream;
    }
}

TEST_CASE("Multivector printing", "[ga]") {
    const auto e1 = Multivector<double, metric, 1>(1);
    const auto e3 = Multivector<double, metric, 2>(1);
    const auto e5 = Multivector<double, metric, 4>(1);
    const auto e13 = e1^e3;
    const auto e135 = e1^e3^e5;
    auto stream = std::stringstream();
    stream << -3 + 3*e1 + e3 - e5 + 2*e13 + e135;
    REQUIRE(stream.str() == "-3 + 3e1 + e3 - e5 + 2e13 + e135");
    stream.str("");
    stream << 3*e1;
    REQUIRE(stream.str() == "3e1");
    stream.str("");
    stream << -3*e1;
    REQUIRE(stream.str() == "-3e1");
    stream.str("");
    stream << (1 + e1 - e1);
    REQUIRE(stream.str() == "1");
    stream.str("");
    stream << (-1 + e1 - e1);
    REQUIRE(stream.str() == "-1");
    stream.str("");
    stream << (e1 - e1);
    REQUIRE(stream.str() == "0");
}

TEST_CASE("3D PGA printing", "[ga]") {
    using namespace ganim::pga3;
    auto stream = std::stringstream();
    stream << 1 + 2*e1 - 3*e0123;
    REQUIRE(stream.str() == "1 + 2e1 - 3e0123");
}
