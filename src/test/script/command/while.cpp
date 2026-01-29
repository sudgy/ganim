#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

TEST_CASE("While", "[script]") {
    auto script = Script(R"(
var a = 0;
var b = 10;
while a < 10 {
    a += 1;
    b += 1;
}
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    auto b = script.symbol_table().get_variable("b");
    REQUIRE(*a->value().get_as<std::int64_t>() == 10);
    REQUIRE(*b->value().get_as<std::int64_t>() == 20);
}
