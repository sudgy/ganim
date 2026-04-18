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

TEST_CASE("Functions using globals", "[script]") {
    auto test = run_script(R"(
var a = 0;
function f(b : int) : void
{
    a = b;
}
function g() : void
{
    test_output(a);
}
g();
f(1);
g();
g();
f(2);
f(3);
g();
    )", 8);
    REQUIRE(test.size() == 4);
    REQUIRE(get<int64_t>(test[0]) == 0);
    REQUIRE(get<int64_t>(test[1]) == 1);
    REQUIRE(get<int64_t>(test[2]) == 1);
    REQUIRE(get<int64_t>(test[3]) == 3);
}

TEST_CASE("Function scoping", "[script]") {
    auto test = run_script(R"(
var a = 0;
if true {
    var a = 1;
    function f() : void
    {
        test_output(a);
        if true {
            test_output(a);
        }
        var a = 2;
        test_output(a);
        if true {
            test_output(a);
        }
    }
    f();
}
    )", 8);
    REQUIRE(test.size() == 4);
    REQUIRE(get<int64_t>(test[0]) == 0);
    REQUIRE(get<int64_t>(test[1]) == 0);
    REQUIRE(get<int64_t>(test[2]) == 2);
    REQUIRE(get<int64_t>(test[3]) == 2);
}

TEST_CASE("Function called at different scopes", "[script]") {
    auto test = run_script(R"(
var a = 2;
function f(b : int) : void
{
    var c = 3;
    test_output(a + b + c);
}
function g(b : int) : void
{
    f(b);
}
var d1 = 0;
f(1);
var d2 = 0;
f(2);
if true {
    f(3);
    var d3 = 0;
    g(4);
    if true {
        g(5);
        var d4 = 0;
        f(6);
    }
    g(7);
}
g(8);
    )", 24);
    REQUIRE(test.size() == 8);
    REQUIRE(get<int64_t>(test[0]) == 6);
    REQUIRE(get<int64_t>(test[1]) == 7);
    REQUIRE(get<int64_t>(test[2]) == 8);
    REQUIRE(get<int64_t>(test[3]) == 9);
    REQUIRE(get<int64_t>(test[4]) == 10);
    REQUIRE(get<int64_t>(test[5]) == 11);
    REQUIRE(get<int64_t>(test[6]) == 12);
    REQUIRE(get<int64_t>(test[7]) == 13);
}

// Add return types
