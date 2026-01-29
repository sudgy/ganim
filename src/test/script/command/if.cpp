#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

TEST_CASE("If", "[script]") {
    auto script = Script(R"(
var a = 5;
if a == 5 {
    a = 7;
    a = 6;
}
if (3.0 > 4.0) {
    a = 7;
}
if true {}
if true {
    if false {
        a = 8;
    }
}
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 6);
}

TEST_CASE("Else", "[script]") {
    auto script = Script(R"(
var a = 5;
if a == 5 {
    a = 6;
}
else {
    a = 7;
}
var b = 5;
if a == 5 {
    b = 6;
}
else {
    b = 7;
}
var c = 5;
if true {
    if false {
        c = 6;
    }
    else {
        c = 7;
    }
}
else {
    c = 8;
}
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 6);
    auto b = script.symbol_table().get_variable("b");
    REQUIRE( b->value().get_as<std::int64_t>());
    REQUIRE(*b->value().get_as<std::int64_t>() == 7);
    auto c = script.symbol_table().get_variable("c");
    REQUIRE( c->value().get_as<std::int64_t>());
    REQUIRE(*c->value().get_as<std::int64_t>() == 7);
}

TEST_CASE("Else if", "[script]") {
    auto script = Script(R"(
var a = 5;
if true {
    a = 6;
}
else if true {
    a = 7;
}
var b = 5;
if false {
    b = 6;
}
else if true {
    b = 7;
}
var c = 5;
if false {
    c = 6;
}
else if false {
    c = 7;
}
else {
    c = 8;
}
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 6);
    auto b = script.symbol_table().get_variable("b");
    REQUIRE( b->value().get_as<std::int64_t>());
    REQUIRE(*b->value().get_as<std::int64_t>() == 7);
    auto c = script.symbol_table().get_variable("c");
    REQUIRE( c->value().get_as<std::int64_t>());
    REQUIRE(*c->value().get_as<std::int64_t>() == 8);
}

TEST_CASE("If scope", "[script]") {
    auto script = Script(R"(
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
    )");
    script.compile();
    script.execute();
    auto a1 = script.symbol_table().get_variable("a1");
    auto a2 = script.symbol_table().get_variable("a2");
    auto b = script.symbol_table().get_variable("b");
    REQUIRE(*a1->value().get_as<std::int64_t>() == 5);
    REQUIRE(*a2->value().get_as<std::int64_t>() == 4);
    REQUIRE(*b->value().get_as<std::int64_t>() == 9);
}
