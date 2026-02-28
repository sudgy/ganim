#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

// 2 + 3

TEST_CASE("compile test_output", "[script]") {
    auto output = run_script(R"(
test_output(2);
//test_output(-2);
test_output(0x200);
//test_output(-0x200);
test_output(0x20000);
//test_output(-0x20000);
test_output(0x200000000);
//test_output(-0x200000000);
test_output(5.0);
test_output(true);
    )");
    //REQUIRE(output.size() == 10);
    //REQUIRE(get<std::int64_t>(output[0]) == 2);
    //REQUIRE(get<std::int64_t>(output[1]) == -2);
    //REQUIRE(get<std::int64_t>(output[2]) == 0x200);
    //REQUIRE(get<std::int64_t>(output[3]) == -0x200);
    //REQUIRE(get<std::int64_t>(output[4]) == 0x20000);
    //REQUIRE(get<std::int64_t>(output[5]) == -0x20000);
    //REQUIRE(get<std::int64_t>(output[6]) == 0x200000000);
    //REQUIRE(get<std::int64_t>(output[7]) == -0x200000000);
    //REQUIRE(get<double>(output[8]) == 5.0);
    //REQUIRE(get<std::byte>(output[9]) == std::byte(1));
    REQUIRE(output.size() == 6);
    REQUIRE(get<std::int64_t>(output[0]) == 2);
    REQUIRE(get<std::int64_t>(output[1]) == 0x200);
    REQUIRE(get<std::int64_t>(output[2]) == 0x20000);
    REQUIRE(get<std::int64_t>(output[3]) == 0x200000000);
    REQUIRE(get<double>(output[4]) == 5.0);
    REQUIRE(get<std::byte>(output[5]) == std::byte(1));
}
