#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Setting variables", "[script]") {
    auto test = run_script(R"(
var a = 5;
a = 10;
test_output(a);
    )", 8);
    REQUIRE(test.size() == 1);
    REQUIRE(get<int64_t>(test[0]) == 10);

    REQUIRE_THROWS(run_script("a = 10;"));
    REQUIRE_THROWS(run_script("var a = 5; a = true;"));
}
