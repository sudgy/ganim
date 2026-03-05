#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Setting variables", "[script]") {
    auto test = run_script(R"(
var a = 5;
a = 10;
test_output(a);
    )", 8);
    REQUIRE(test.size() == 1);
    REQUIRE(get<int64_t>(test[0]) == 10);

    REQUIRE_THROWS(run_script("a = 10;"));
    REQUIRE_THROWS(run_script("var a = 5; a = true;"));
}

TEST_CASE("Arithmetic assignment operators", "[script]") {
    auto test = run_script(R"(
var a = 5;
a += 2;
a -= 3;
a *= 2;
a /= 3;
var b = 5.0;
b /= 2.0;
test_output(a);
test_output(b);
    )", 16);
    REQUIRE_THROWS(run_script("var a = 5; a += true;"));
    REQUIRE_THROWS(run_script("var a = true; a += false;"));

    REQUIRE(test.size() == 2);
    REQUIRE(get<int64_t>(test[0]) == 2);
    REQUIRE(get<double>(test[1]) == 2.5);
}
