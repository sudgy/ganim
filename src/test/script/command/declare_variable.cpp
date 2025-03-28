#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/command/declare_variable.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;

TEST_CASE("DeclareVariable", "[script]") {
    auto script = Script("var a = 5;");
    script.compile();
    auto v1 = script.get_variable("a");
    auto v2 = script.get_variable("b");
    REQUIRE(v1 != nullptr);
    REQUIRE(v2 == nullptr);
    REQUIRE(v1->type() == any_pointer::get_tag<std::int64_t>());
    REQUIRE(v1->value().get_tag() == any_pointer::get_tag<std::int64_t>());
    REQUIRE(v1->value().get_as<std::int64_t>() == nullptr);

    script.execute();
    REQUIRE(v1->value().get_as<std::int64_t>() != nullptr);
    REQUIRE(*v1->value().get_as<std::int64_t>() == 5);
}
