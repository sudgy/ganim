#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

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
