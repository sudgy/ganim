#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

TEST_CASE("DeclareVariable", "[script]") {
    auto script = Script("var a = 5;");
    script.compile();
    auto v1 = script.get_variable("a");
    auto v2 = script.get_variable("b");
    REQUIRE(v1 != nullptr);
    REQUIRE(v2 == nullptr);
    REQUIRE(v1->type() == Type{any_pointer::get_tag<std::int64_t>()});
    REQUIRE(v1->value().get_tag() == any_pointer::get_tag<std::int64_t>());
    REQUIRE(v1->value().get_as<std::int64_t>() == nullptr);

    script.execute();
    REQUIRE(v1->value().get_as<std::int64_t>() != nullptr);
    REQUIRE(*v1->value().get_as<std::int64_t>() == 5);
}

TEST_CASE("Complicated arithmetic expressions", "[script]") {
    auto script = Script(R"(
var a = 5;
var b = 3;
var c = b + (a + 3) * b + 4 / (5 % 3) - 3;
    )");
    script.compile();
    script.execute();
    auto test = script.get_variable("c");
    REQUIRE(test);
    REQUIRE(test->value().get_as<std::int64_t>());
    REQUIRE(*test->value().get_as<std::int64_t>() == 26);
}

TEST_CASE("Unary plus/minus", "[script]") {
    auto script = Script(R"(
var a = +5;
var b = -3;
var c = 3 + - 4;
var d = 3 ++--+4;
    )");
    script.compile();
    script.execute();
    auto test1 = script.get_variable("a");
    auto test2 = script.get_variable("b");
    auto test3 = script.get_variable("c");
    auto test4 = script.get_variable("d");
    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(test2->value().get_as<std::int64_t>());
    REQUIRE(test3->value().get_as<std::int64_t>());
    REQUIRE(test4->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 5);
    REQUIRE(*test2->value().get_as<std::int64_t>() == -3);
    REQUIRE(*test3->value().get_as<std::int64_t>() == -1);
    REQUIRE(*test4->value().get_as<std::int64_t>() == 7);
}

TEST_CASE("Constants", "[script]") {
    auto script = Script(R"(
let a = 5;
let b = a + 3;
    )");
    script.compile();
    script.execute();
    auto a = script.get_variable("a");
    auto b = script.get_variable("b");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 5);
    REQUIRE( b->value().get_as<std::int64_t>());
    REQUIRE(*b->value().get_as<std::int64_t>() == 8);

    auto bad_script = Script("let a = 5; a = 10;");
    REQUIRE_THROWS(bad_script.compile());
}

TEST_CASE("Type specifiers", "[script]") {
    auto script = Script(R"(
let a = 5 : int;
let b = 4.0 : double;
let c = "foo" : string;
let d = true : bool;
    )");
    auto bad_script1 = Script("let a = 5 : string;");
    auto bad_script2 = Script("let a = 5 : foo;");
    REQUIRE_NOTHROW(script.compile());
    REQUIRE_THROWS(bad_script1.compile());
    REQUIRE_THROWS(bad_script2.compile());
}
