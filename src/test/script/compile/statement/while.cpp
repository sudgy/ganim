#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("While", "[script]") {
    auto test = run_script(R"(
var a = 0;
while a < 10 {
    a += 1;
    var b = 0;
    b += 1;
    test_output(a);
    test_output(b);
}
    )", 8);
    REQUIRE(test.size() == 20);
    for (int i = 0; i < 10; ++i) {
        INFO("i = " << i);
        REQUIRE(get<int64_t>(test[2*i]) == i + 1);
        REQUIRE(get<int64_t>(test[2*i+1]) == 1);
    }
}
