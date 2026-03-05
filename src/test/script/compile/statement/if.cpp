#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("If", "[script]") {
    auto test = run_script(R"(
var a = 5;
if a == 5 {
    test_output(1);
}
if (3.0 > 4.0) {
    test_output(2);
}
if true {}
if true {
    if false {
        test_output(3);
    }
    test_output(4);
}
    )", 8);
    REQUIRE(test.size() == 2);
    REQUIRE(get<int64_t>(test[0]) == 1);
    REQUIRE(get<int64_t>(test[1]) == 4);
}

TEST_CASE("Else", "[script]") {
    auto test = run_script(R"(
var a = 5;
if a == 5 {
    test_output(1);
}
else {
    test_output(2);
}
if a == 6 {
    test_output(3);
}
else {
    test_output(4);
}
if true {
    if false {
        test_output(5);
    }
    else {
        test_output(6);
    }
}
else {
    test_output(7);
}
if true {}
else {
    test_output(8);
}
if false {}
else {
    test_output(9);
}
    )", 8);
    REQUIRE(test.size() == 4);
    REQUIRE(get<int64_t>(test[0]) == 1);
    REQUIRE(get<int64_t>(test[1]) == 4);
    REQUIRE(get<int64_t>(test[2]) == 6);
    REQUIRE(get<int64_t>(test[3]) == 9);
}

TEST_CASE("Else if", "[script]") {
    auto test = run_script(R"(
if true {
    test_output(1);
}
else if true {
    test_output(2);
}
if false {
    test_output(3);
}
else if true {
    test_output(4);
}
if false {
    test_output(5);
}
else if false {
    test_output(6);
}
else {
    test_output(7);
}
    )");
    REQUIRE(test.size() == 3);
    REQUIRE(get<int64_t>(test[0]) == 1);
    REQUIRE(get<int64_t>(test[1]) == 4);
    REQUIRE(get<int64_t>(test[2]) == 7);
}

TEST_CASE("If scope", "[script]") {
    auto test = run_script(R"(
var a1 = 5;
var a2 = 5;
if true {
    var a1 = 6;
    a1 = 7;
}
else {
    var b = 8;
}
var b = 9;
if false {}
else {
    var b = 10;
    a2 = 4;
}
test_output(a1);
test_output(a2);
test_output(b);
    )", 24);
    REQUIRE(test.size() == 3);
    REQUIRE(get<int64_t>(test[0]) == 5);
    REQUIRE(get<int64_t>(test[1]) == 4);
    REQUIRE(get<int64_t>(test[2]) == 9);
    REQUIRE_THROWS(run_script(R"(
if true {
    var a = 5;
}
a = 6;
    )"));
}
