#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/command/print.hpp"
#include "script/script.hpp"

#include "script/script_exception.hpp"

using namespace ganim;
using namespace ganim::commands;

TEST_CASE("Print", "[script]") {
    auto results = std::vector<std::string>();
    Print::set_print_function([&](std::string_view s) {
        results.emplace_back(s);
    });
    auto script = Script(R"(
print "Hello, world!";
print "Another one!";
print 42;
print true;
print false;
print 1e2;
print 1e100;
print 6.28;
print .5;
    )");
    script.compile();
    script.execute();
    REQUIRE(results.size() == 9);
    REQUIRE(results[0] == "Hello, world!");
    REQUIRE(results[1] == "Another one!");
    REQUIRE(results[2] == "42");
    REQUIRE(results[3] == "true");
    REQUIRE(results[4] == "false");
    REQUIRE(results[5] == "100");
    REQUIRE(results[6] == "1e+100");
    REQUIRE(results[7] == "6.28");
    REQUIRE(results[8] == "0.5");
}

TEST_CASE("Print error handling", "[script]") {
    auto script = Script("print;");
    REQUIRE_THROWS_AS(script.compile(), CompileError);

    auto script2 = Script("print \"Two\" \"Things\";");
    REQUIRE_THROWS_AS(script2.compile(), CompileError);

    auto script3 = Script("print \"foo\"");
    REQUIRE_THROWS_AS(script3.compile(), CompileError);
}
