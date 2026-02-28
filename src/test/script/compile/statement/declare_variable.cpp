#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("DeclareVariable", "[script]") {
    auto test = run_script(R"(
var a = 5;
test_output(a);
var b = 6;
test_output(a);
test_output(b);
)");
    REQUIRE(test.size() == 3);
    REQUIRE(get<std::int64_t>(test[0]) == 5);
    REQUIRE(get<std::int64_t>(test[1]) == 5);
    REQUIRE(get<std::int64_t>(test[2]) == 6);
}
