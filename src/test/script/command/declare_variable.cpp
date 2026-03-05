#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

using namespace ganim;

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
