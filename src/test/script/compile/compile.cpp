#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("compile test_output", "[script]") {
    auto output = run_script(R"(
test_output(2);
test_output(-2);
test_output(0x200);
test_output(-0x200);
test_output(0x20000);
test_output(-0x20000);
test_output(0x200000000);
test_output(-0x200000000);
test_output(5.0);
test_output(true);
    )");
    REQUIRE(output.size() == 10);
    REQUIRE(get<std::int64_t>(output[0]) == 2);
    REQUIRE(get<std::int64_t>(output[1]) == -2);
    REQUIRE(get<std::int64_t>(output[2]) == 0x200);
    REQUIRE(get<std::int64_t>(output[3]) == -0x200);
    REQUIRE(get<std::int64_t>(output[4]) == 0x20000);
    REQUIRE(get<std::int64_t>(output[5]) == -0x20000);
    REQUIRE(get<std::int64_t>(output[6]) == 0x200000000);
    REQUIRE(get<std::int64_t>(output[7]) == -0x200000000);
    REQUIRE(get<double>(output[8]) == 5.0);
    REQUIRE(get<std::byte>(output[9]) == std::byte(1));
}

TEST_CASE("Script error handling", "[script]") {
    // Just check that an empty script works
    // Commented out because currently the thing that this is checking lives in
    // the test code which doesn't need to get tested.  Maybe uncomment this
    // when the rest of the framework is built?
    //auto script = run_script("");

    REQUIRE_THROWS(run_script("  foobar"));
}

TEST_CASE("Comments", "[script]") {
    auto test = run_script(R"(
/**/
var a = 5; // Single line comment
var b = 6; /* Multi line comment
b = 7;
*/ var c = 8;
test_output(a);
test_output(b);
test_output(c);
)");
    REQUIRE(test.size() == 3);
    REQUIRE(get<int64_t>(test[0]) == 5);
    REQUIRE(get<int64_t>(test[1]) == 6);
    REQUIRE(get<int64_t>(test[2]) == 8);
    REQUIRE_THROWS(run_script("/*"));
    REQUIRE_THROWS(run_script("*/"));
    REQUIRE_THROWS(run_script("/*/"));
}
