#include <catch2/catch_test_macros.hpp>

//#include "script/expression/variable.hpp"
#include "script/script.hpp"

using namespace ganim;

TEST_CASE("Boolean expressions", "[script]") {
    auto script = Script(R"(
var a1 = 3 < 4;
var a2 = 3 <= 4;
var a3 = 3 == 4;
var a4 = 3 != 4;
var a5 = 3 >= 4;
var a6 = 3 > 4;
var b1 = 4 < 4;
var b2 = 4 <= 4;
var b3 = 4 == 4;
var b4 = 4 != 4;
var b5 = 4 >= 4;
var b6 = 4 > 4;
var c1 = 5 < 4;
var c2 = 5 <= 4;
var c3 = 5 == 4;
var c4 = 5 != 4;
var c5 = 5 >= 4;
var c6 = 5 > 4;
var d1 = 5.0 < 4.0;
var d2 = 5.0 <= 4.0;
var d3 = 5.0 == 4.0;
var d4 = 5.0 != 4.0;
var d5 = 5.0 >= 4.0;
var d6 = 5.0 > 4.0;
var e1 = 3 + 2 > 4;
var e2 = 5 <= 3 * 2 - 1;
var f1 = true == true;
var f2 = false == true;
var f3 = true != true;
var f4 = false != true;
var g1 = "foo" == "foo";
var g2 = "foo" == "bar";
var g3 = "foo" != "foo";
var g4 = "foo" != "bar";
    )");
    script.compile();
    script.execute();

    auto bad_script1 = Script("var a = true < true;");
    auto bad_script2 = Script("var a = 5 < \"foo\";");
    REQUIRE_THROWS(bad_script1.compile());
    REQUIRE_THROWS(bad_script2.compile());

    auto a1 = script.get_variable("a1");
    auto a2 = script.get_variable("a2");
    auto a3 = script.get_variable("a3");
    auto a4 = script.get_variable("a4");
    auto a5 = script.get_variable("a5");
    auto a6 = script.get_variable("a6");
    auto b1 = script.get_variable("b1");
    auto b2 = script.get_variable("b2");
    auto b3 = script.get_variable("b3");
    auto b4 = script.get_variable("b4");
    auto b5 = script.get_variable("b5");
    auto b6 = script.get_variable("b6");
    auto c1 = script.get_variable("c1");
    auto c2 = script.get_variable("c2");
    auto c3 = script.get_variable("c3");
    auto c4 = script.get_variable("c4");
    auto c5 = script.get_variable("c5");
    auto c6 = script.get_variable("c6");
    auto d1 = script.get_variable("d1");
    auto d2 = script.get_variable("d2");
    auto d3 = script.get_variable("d3");
    auto d4 = script.get_variable("d4");
    auto d5 = script.get_variable("d5");
    auto d6 = script.get_variable("d6");
    auto e1 = script.get_variable("e1");
    auto e2 = script.get_variable("e2");
    auto f1 = script.get_variable("f1");
    auto f2 = script.get_variable("f2");
    auto f3 = script.get_variable("f3");
    auto f4 = script.get_variable("f4");
    auto g1 = script.get_variable("g1");
    auto g2 = script.get_variable("g2");
    auto g3 = script.get_variable("g3");
    auto g4 = script.get_variable("g4");

    REQUIRE(  a1->value().get_as<bool>());
    REQUIRE( *a1->value().get_as<bool>());
    REQUIRE(  a2->value().get_as<bool>());
    REQUIRE( *a2->value().get_as<bool>());
    REQUIRE(  a3->value().get_as<bool>());
    REQUIRE(!*a3->value().get_as<bool>());
    REQUIRE(  a4->value().get_as<bool>());
    REQUIRE( *a4->value().get_as<bool>());
    REQUIRE(  a5->value().get_as<bool>());
    REQUIRE(!*a5->value().get_as<bool>());
    REQUIRE(  a6->value().get_as<bool>());
    REQUIRE(!*a6->value().get_as<bool>());

    REQUIRE(  b1->value().get_as<bool>());
    REQUIRE(!*b1->value().get_as<bool>());
    REQUIRE(  b2->value().get_as<bool>());
    REQUIRE( *b2->value().get_as<bool>());
    REQUIRE(  b3->value().get_as<bool>());
    REQUIRE( *b3->value().get_as<bool>());
    REQUIRE(  b4->value().get_as<bool>());
    REQUIRE(!*b4->value().get_as<bool>());
    REQUIRE(  b5->value().get_as<bool>());
    REQUIRE( *b5->value().get_as<bool>());
    REQUIRE(  b6->value().get_as<bool>());
    REQUIRE(!*b6->value().get_as<bool>());

    REQUIRE(  c1->value().get_as<bool>());
    REQUIRE(!*c1->value().get_as<bool>());
    REQUIRE(  c2->value().get_as<bool>());
    REQUIRE(!*c2->value().get_as<bool>());
    REQUIRE(  c3->value().get_as<bool>());
    REQUIRE(!*c3->value().get_as<bool>());
    REQUIRE(  c4->value().get_as<bool>());
    REQUIRE( *c4->value().get_as<bool>());
    REQUIRE(  c5->value().get_as<bool>());
    REQUIRE( *c5->value().get_as<bool>());
    REQUIRE(  c6->value().get_as<bool>());
    REQUIRE( *c6->value().get_as<bool>());

    REQUIRE(  d1->value().get_as<bool>());
    REQUIRE(!*d1->value().get_as<bool>());
    REQUIRE(  d2->value().get_as<bool>());
    REQUIRE(!*d2->value().get_as<bool>());
    REQUIRE(  d3->value().get_as<bool>());
    REQUIRE(!*d3->value().get_as<bool>());
    REQUIRE(  d4->value().get_as<bool>());
    REQUIRE( *d4->value().get_as<bool>());
    REQUIRE(  d5->value().get_as<bool>());
    REQUIRE( *d5->value().get_as<bool>());
    REQUIRE(  d6->value().get_as<bool>());
    REQUIRE( *d6->value().get_as<bool>());

    REQUIRE(  e1->value().get_as<bool>());
    REQUIRE( *e1->value().get_as<bool>());
    REQUIRE(  e2->value().get_as<bool>());
    REQUIRE( *e2->value().get_as<bool>());

    REQUIRE(  f1->value().get_as<bool>());
    REQUIRE( *f1->value().get_as<bool>());
    REQUIRE(  f2->value().get_as<bool>());
    REQUIRE(!*f2->value().get_as<bool>());
    REQUIRE(  f3->value().get_as<bool>());
    REQUIRE(!*f3->value().get_as<bool>());
    REQUIRE(  f4->value().get_as<bool>());
    REQUIRE( *f4->value().get_as<bool>());

    REQUIRE(  g1->value().get_as<bool>());
    REQUIRE( *g1->value().get_as<bool>());
    REQUIRE(  g2->value().get_as<bool>());
    REQUIRE(!*g2->value().get_as<bool>());
    REQUIRE(  g3->value().get_as<bool>());
    REQUIRE(!*g3->value().get_as<bool>());
    REQUIRE(  g4->value().get_as<bool>());
    REQUIRE( *g4->value().get_as<bool>());
}
