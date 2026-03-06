#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Loop", "[script]") {
    auto test = run_script(R"(
var a = 0;
loop {
    var b = 5; // Check that stack cleanup works right
    a += 1;
    test_output(a);
    if a == 9 {
        break;
    }
    test_output(a);
    if a == 8 {
        continue;
    }
    test_output(a);
}
    )", 8);
    REQUIRE(test.size() == 24);
    REQUIRE(get<int64_t>(test[0]) == 1);
    REQUIRE(get<int64_t>(test[1]) == 1);
    REQUIRE(get<int64_t>(test[2]) == 1);
    REQUIRE(get<int64_t>(test[3]) == 2);
    REQUIRE(get<int64_t>(test[4]) == 2);
    REQUIRE(get<int64_t>(test[5]) == 2);
    REQUIRE(get<int64_t>(test[6]) == 3);
    REQUIRE(get<int64_t>(test[7]) == 3);
    REQUIRE(get<int64_t>(test[8]) == 3);
    REQUIRE(get<int64_t>(test[9]) == 4);
    REQUIRE(get<int64_t>(test[10]) == 4);
    REQUIRE(get<int64_t>(test[11]) == 4);
    REQUIRE(get<int64_t>(test[12]) == 5);
    REQUIRE(get<int64_t>(test[13]) == 5);
    REQUIRE(get<int64_t>(test[14]) == 5);
    REQUIRE(get<int64_t>(test[15]) == 6);
    REQUIRE(get<int64_t>(test[16]) == 6);
    REQUIRE(get<int64_t>(test[17]) == 6);
    REQUIRE(get<int64_t>(test[18]) == 7);
    REQUIRE(get<int64_t>(test[19]) == 7);
    REQUIRE(get<int64_t>(test[20]) == 7);
    REQUIRE(get<int64_t>(test[21]) == 8);
    REQUIRE(get<int64_t>(test[22]) == 8);
    REQUIRE(get<int64_t>(test[23]) == 9);
}
