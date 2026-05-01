#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Built-in functions", "[script]") {
    auto test = run_script(R"(
test_output(sin(τ));
test_output(sin(τ/4.0));
test_output(cos(τ));
test_output(cos(τ/4.0));
test_output(tan(0.0));
test_output(tan(τ/8.0));
test_output(exp(0.0));
test_output(exp(1.0));
test_output(log(1.0));
test_output(log(e));
    )");
    REQUIRE(test.size() == 10);
    using Catch::Matchers::WithinAbs;
    REQUIRE_THAT(get<double>(test[0]), WithinAbs(0, 1e-5));
    REQUIRE_THAT(get<double>(test[1]), WithinAbs(1, 1e-5));
    REQUIRE_THAT(get<double>(test[2]), WithinAbs(1, 1e-5));
    REQUIRE_THAT(get<double>(test[3]), WithinAbs(0, 1e-5));
    REQUIRE_THAT(get<double>(test[4]), WithinAbs(0, 1e-5));
    REQUIRE_THAT(get<double>(test[5]), WithinAbs(1, 1e-5));
    REQUIRE_THAT(get<double>(test[6]), WithinAbs(1, 1e-5));
    REQUIRE_THAT(get<double>(test[7]), WithinAbs(std::numbers::e, 1e-5));
    REQUIRE_THAT(get<double>(test[8]), WithinAbs(0, 1e-5));
    REQUIRE_THAT(get<double>(test[9]), WithinAbs(1, 1e-5));
}
