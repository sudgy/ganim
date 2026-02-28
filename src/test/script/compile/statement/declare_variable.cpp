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
var c = b + (a + 3) * b + 4 / (5 % 3) - 3;
test_output(c);
    )");
    REQUIRE(test.size() == 1);
    REQUIRE(get<int64_t>(test[0]) == 26);
}
