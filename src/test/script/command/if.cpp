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
