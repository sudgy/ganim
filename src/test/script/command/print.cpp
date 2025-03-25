#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/command/print.hpp"
#include "script/script.hpp"

#include "script/script_exception.hpp"

using namespace ganim;

// I haven't made a way to change what Print outputs to so I won't bother
// writing tests for that until I do that

TEST_CASE("Print error handling") {
    auto script = Script("print;");
    REQUIRE_THROWS_AS(script.compile(), ScriptException);

    auto script2 = Script("print \"Two\" \"Things\";");
    REQUIRE_THROWS_AS(script2.compile(), ScriptException);

    auto script3 = Script("print \"foo\"");
    REQUIRE_THROWS_AS(script3.compile(), ScriptException);
}
