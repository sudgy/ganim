#include <catch2/catch_test_macros.hpp>

#include "script/script.hpp"

using namespace ganim;

TEST_CASE("Boolean operators", "[script]") {
    auto script = Script(R"(
var a1 = true  and true;
var a2 = true  and false;
var a3 = false and false;
var b1 = true  or true;
var b2 = true  or false;
var b3 = false or false;
var c1 = not true;
var c2 = not false;
var d1 = true  xor true;
var d2 = true  xor false;
var d3 = false xor false;
var e1 = true  nand true;
var e2 = true  nand false;
var e3 = false nand false;
var f1 = true  nor true;
var f2 = true  nor false;
var f3 = false nor false;
var g1 = ¬true;
var h1 = true && false;
var h2 = true || false;
var h3 = !true;
var h4 = true ^ false;
    )");
    script.compile();
    script.execute();

    auto bad_script = Script("true and 4;");
    REQUIRE_THROWS(bad_script.compile());

    auto a1 = script.symbol_table().get_variable("a1");
    auto a2 = script.symbol_table().get_variable("a2");
    auto a3 = script.symbol_table().get_variable("a3");
    auto b1 = script.symbol_table().get_variable("b1");
    auto b2 = script.symbol_table().get_variable("b2");
    auto b3 = script.symbol_table().get_variable("b3");
    auto c1 = script.symbol_table().get_variable("c1");
    auto c2 = script.symbol_table().get_variable("c2");
    auto d1 = script.symbol_table().get_variable("d1");
    auto d2 = script.symbol_table().get_variable("d2");
    auto d3 = script.symbol_table().get_variable("d3");
    auto e1 = script.symbol_table().get_variable("e1");
    auto e2 = script.symbol_table().get_variable("e2");
    auto e3 = script.symbol_table().get_variable("e3");
    auto f1 = script.symbol_table().get_variable("f1");
    auto f2 = script.symbol_table().get_variable("f2");
    auto f3 = script.symbol_table().get_variable("f3");
    auto g1 = script.symbol_table().get_variable("g1");
    auto h1 = script.symbol_table().get_variable("h1");
    auto h2 = script.symbol_table().get_variable("h2");
    auto h3 = script.symbol_table().get_variable("h3");
    auto h4 = script.symbol_table().get_variable("h4");

    REQUIRE(  a1->value().get_as<bool>());
    REQUIRE( *a1->value().get_as<bool>());
    REQUIRE(  a2->value().get_as<bool>());
    REQUIRE(!*a2->value().get_as<bool>());
    REQUIRE(  a3->value().get_as<bool>());
    REQUIRE(!*a3->value().get_as<bool>());

    REQUIRE(  b1->value().get_as<bool>());
    REQUIRE( *b1->value().get_as<bool>());
    REQUIRE(  b2->value().get_as<bool>());
    REQUIRE( *b2->value().get_as<bool>());
    REQUIRE(  b3->value().get_as<bool>());
    REQUIRE(!*b3->value().get_as<bool>());

    REQUIRE(  c1->value().get_as<bool>());
    REQUIRE(!*c1->value().get_as<bool>());
    REQUIRE(  c2->value().get_as<bool>());
    REQUIRE( *c2->value().get_as<bool>());

    REQUIRE(  d1->value().get_as<bool>());
    REQUIRE(!*d1->value().get_as<bool>());
    REQUIRE(  d2->value().get_as<bool>());
    REQUIRE( *d2->value().get_as<bool>());
    REQUIRE(  d3->value().get_as<bool>());
    REQUIRE(!*d3->value().get_as<bool>());

    REQUIRE(  e1->value().get_as<bool>());
    REQUIRE(!*e1->value().get_as<bool>());
    REQUIRE(  e2->value().get_as<bool>());
    REQUIRE( *e2->value().get_as<bool>());
    REQUIRE(  e3->value().get_as<bool>());
    REQUIRE( *e3->value().get_as<bool>());

    REQUIRE(  f1->value().get_as<bool>());
    REQUIRE(!*f1->value().get_as<bool>());
    REQUIRE(  f2->value().get_as<bool>());
    REQUIRE(!*f2->value().get_as<bool>());
    REQUIRE(  f3->value().get_as<bool>());
    REQUIRE( *f3->value().get_as<bool>());

    REQUIRE(  g1->value().get_as<bool>());
    REQUIRE(!*g1->value().get_as<bool>());

    REQUIRE(  h1->value().get_as<bool>());
    REQUIRE(!*h1->value().get_as<bool>());
    REQUIRE(  h2->value().get_as<bool>());
    REQUIRE( *h2->value().get_as<bool>());
    REQUIRE(  h3->value().get_as<bool>());
    REQUIRE(!*h3->value().get_as<bool>());
    REQUIRE(  h4->value().get_as<bool>());
    REQUIRE( *h4->value().get_as<bool>());
}
