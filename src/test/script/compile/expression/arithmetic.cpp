#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Binary expressions", "[script]") {
    auto test = run_script(R"(
test_output(3 + 4);
test_output(5.0 + 6.0);
test_output(3 * 4);
test_output(5.0 * 6.0);
test_output(4 / 3);
test_output(5.0 / 2.0);
test_output(2 - 3);
test_output(3.0 - 5.0);
test_output(5 % 2);
    )");
    REQUIRE_THROWS(run_script("var a = true + false;"));
    REQUIRE_THROWS(run_script("var a = 3 + true;"));
    REQUIRE_THROWS(run_script("var a = 3.0 % 2.0;"));

    REQUIRE(test.size() == 9);
    REQUIRE(get<int64_t>(test[0]) == 7);
    REQUIRE(get<double>(test[1]) == 11.0);
    REQUIRE(get<int64_t>(test[2]) == 12);
    REQUIRE(get<double>(test[3]) == 30.0);
    REQUIRE(get<int64_t>(test[4]) == 1);
    REQUIRE(get<double>(test[5]) == 2.5);
    REQUIRE(get<int64_t>(test[6]) == -1);
    REQUIRE(get<double>(test[7]) == -2.0);
    REQUIRE(get<int64_t>(test[8]) == 1);
}

TEST_CASE("UnaryPlus expressions", "[script]") {
    auto test = run_script(R"(
test_output(+3);
test_output(+4.0);
    )");
    REQUIRE_THROWS(run_script("var a = +true;"));
    REQUIRE(test.size() == 2);
    REQUIRE(get<int64_t>(test[0]) == 3);
    REQUIRE(get<double>(test[1]) == 4.0);
}

TEST_CASE("UnaryMinus expressions", "[script]") {
    auto test = run_script(R"(
test_output(-3);
test_output(-4.0);
    )");
    REQUIRE_THROWS(run_script("var a = -true;"));
    REQUIRE(test.size() == 2);
    REQUIRE(get<int64_t>(test[0]) == -3);
    REQUIRE(get<double>(test[1]) == -4.0);
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
