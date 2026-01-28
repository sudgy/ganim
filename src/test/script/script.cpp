#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

#include "script/script_exception.hpp"

using namespace ganim;

TEST_CASE("Script error handling", "[script]") {
    // Just check that an empty script works
    auto script = Script("");
    script.compile();
    script.execute();

    auto script2 = Script("  foobar");
    REQUIRE_THROWS(script2.compile());
}

TEST_CASE("Comments", "[script]") {
    auto script = Script(R"(
/**/
var a = 5; // Single line comment
var b = 6; /* Multi line comment
b = 7;
*/ var c = 8;
)");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    auto b = script.symbol_table().get_variable("b");
    auto c = script.symbol_table().get_variable("c");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 5);
    REQUIRE( b->value().get_as<std::int64_t>());
    REQUIRE(*b->value().get_as<std::int64_t>() == 6);
    REQUIRE( c->value().get_as<std::int64_t>());
    REQUIRE(*c->value().get_as<std::int64_t>() == 8);
    auto bad_script1 = Script("/*");
    auto bad_script2 = Script("*/");
    auto bad_script3 = Script("/*/");
    REQUIRE_THROWS(bad_script1.compile());
    REQUIRE_THROWS(bad_script2.compile());
    REQUIRE_THROWS(bad_script3.compile());
}
