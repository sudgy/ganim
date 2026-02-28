#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

TEST_CASE("Constants", "[script]") {
    auto script = Script(R"(
let a = 5;
let b = a + 3;
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    auto b = script.symbol_table().get_variable("b");
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
