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
