#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

TEST_CASE("Setting variables", "[script]") {
    auto script = Script(R"(
var a = 5;
a = 10;
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 10);

    auto bad_script1 = Script("a = 10;");
    auto bad_script2 = Script("var a = 5; a = true;");
    REQUIRE_THROWS(bad_script1.compile());
    REQUIRE_THROWS(bad_script2.compile());
}

TEST_CASE("Arithmetic assignment operators", "[script]") {
    auto script = Script(R"(
var a = 5;
a += 2;
a -= 3;
a *= 2;
a /= 3;
var b = 5.0;
b /= 2.0;
var c = "foo";
c += "bar";
    )");
    auto bad_script1 = Script("var a = \"foo\"; a -= \"bar\";");
    auto bad_script2 = Script("var a = 5; a += \"foo\";");
    auto bad_script3 = Script("var a = true; a += false;");
    REQUIRE_THROWS(bad_script1.compile());
    REQUIRE_THROWS(bad_script2.compile());
    REQUIRE_THROWS(bad_script3.compile());

    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 2);
    auto b = script.symbol_table().get_variable("b");
    REQUIRE( b->value().get_as<double>());
    REQUIRE(*b->value().get_as<double>() == 2.5);
    auto c = script.symbol_table().get_variable("c");
    REQUIRE( c->value().get_as<std::string>());
    REQUIRE(*c->value().get_as<std::string>() == "foobar");
}
