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
)", 16);
    REQUIRE(test.size() == 3);
    REQUIRE(get<int64_t>(test[0]) == 5);
    REQUIRE(get<int64_t>(test[1]) == 5);
    REQUIRE(get<int64_t>(test[2]) == 6);
}

TEST_CASE("Constants", "[script]") {
    auto test = run_script(R"(
let a = 5;
let b = a + 3;
test_output(a);
test_output(b);
    )", 16);
    REQUIRE(test.size() == 2);
    REQUIRE(get<int64_t>(test[0]) == 5);
    REQUIRE(get<int64_t>(test[1]) == 8);

    REQUIRE_THROWS(run_script("let a = 5; a = 10;"));
}
