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
