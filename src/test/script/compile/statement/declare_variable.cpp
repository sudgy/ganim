#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("DeclareVariable", "[script]") {
    auto test = run_script(R"(
var a = 5;
test_output(a);
var b = 6;
test_output(a);
test_output(b);
)");
    REQUIRE(test.size() == 3);
    REQUIRE(get<int64_t>(test[0]) == 5);
    REQUIRE(get<int64_t>(test[1]) == 5);
    REQUIRE(get<int64_t>(test[2]) == 6);
}

TEST_CASE("Complicated arithmetic expressions", "[script]") {
    auto test = run_script(R"(
var a = 5;
var b = 3;
test_output(b + (a + 3) * b + 4 / (5 % 3) - 3);
    )");
    REQUIRE(test.size() == 1);
    REQUIRE(get<int64_t>(test[0]) == 26);
}

TEST_CASE("Unary plus/minus", "[script]") {
    auto test = run_script(R"(
test_output(+5);
test_output(-3);
test_output(3 + - 4);
test_output(3 ++--+4);
    )");
    REQUIRE(test.size() == 4);
    REQUIRE(get<int64_t>(test[0]) == 5);
    REQUIRE(get<int64_t>(test[1]) == -3);
    REQUIRE(get<int64_t>(test[2]) == -1);
    REQUIRE(get<int64_t>(test[3]) == 7);
}
