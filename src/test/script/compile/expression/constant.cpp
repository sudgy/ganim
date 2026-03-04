#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Numeric expressions", "[script]") {
    auto test = run_script(R"(
test_output(523);
test_output(0xFF);
test_output(010);
test_output(0b11);
test_output(0.5);
test_output(0);
    )");
    REQUIRE(test.size() == 6);
    REQUIRE(get<int64_t>(test[0]) == 523);
    REQUIRE(get<int64_t>(test[1]) == 0xFF);
    REQUIRE(get<int64_t>(test[2]) == 010);
    REQUIRE(get<int64_t>(test[3]) == 0b11);
    REQUIRE(get<double>(test[4]) == 0.5);
    REQUIRE(get<int64_t>(test[5]) == 0);
}
