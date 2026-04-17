#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Script functions", "[script]") {
    auto test = run_script(R"(
function f1() : void
{
    test_output(1);
}
function f2(a : int) : void
{
    test_output(a);
}
f1();
function f3(a : int, b : int) : void
{
    test_output(a + b);
}
f2(2);
f3(3, 4);
    )");
    REQUIRE(test.size() == 3);
    REQUIRE(get<int64_t>(test[0]) == 1);
    REQUIRE(get<int64_t>(test[1]) == 2);
    REQUIRE(get<int64_t>(test[2]) == 7);

    REQUIRE_THROWS(run_script(R"(
function f(a : int, b : int) : void {}
f(5);
    )"));
    REQUIRE_THROWS(run_script(R"(
function f(a : int) : void {}
f(5, 6);
    )"));
    REQUIRE_THROWS(run_script(R"(
function f(a : bool) : void {}
f(5);
    )"));
}

// Check for type matching
// Add return types
// Test scoping and access rules
