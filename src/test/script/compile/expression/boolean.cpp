#include <catch2/catch_test_macros.hpp>

#include "test/script/run_script.hpp"

using namespace ganim;

TEST_CASE("Relational expressions", "[script]") {
    auto test = run_script(R"(
test_output(3 < 4);
test_output(3 <= 4);
test_output(3 == 4);
test_output(3 != 4);
test_output(3 >= 4);
test_output(3 > 4);
test_output(4 < 4);
test_output(4 <= 4);
test_output(4 == 4);
test_output(4 != 4);
test_output(4 >= 4);
test_output(4 > 4);
test_output(5 < 4);
test_output(5 <= 4);
test_output(5 == 4);
test_output(5 != 4);
test_output(5 >= 4);
test_output(5 > 4);
test_output(5.0 < 4.0);
test_output(5.0 <= 4.0);
test_output(5.0 == 4.0);
test_output(5.0 != 4.0);
test_output(5.0 >= 4.0);
test_output(5.0 > 4.0);
test_output(3 + 2 > 4);
test_output(5 <= 3 * 2 - 1);
test_output(true == true);
test_output(false == true);
test_output(true != true);
test_output(false != true);
    )");
    REQUIRE_THROWS(run_script("var a = true < true;"));
    REQUIRE_THROWS(run_script("var a = 5 < true;"));

    REQUIRE(test.size() == 30);
    REQUIRE(get<std::byte>(test[0]) == std::byte(1));
    REQUIRE(get<std::byte>(test[1]) == std::byte(1));
    REQUIRE(get<std::byte>(test[2]) == std::byte(0));
    REQUIRE(get<std::byte>(test[3]) == std::byte(1));
    REQUIRE(get<std::byte>(test[4]) == std::byte(0));
    REQUIRE(get<std::byte>(test[5]) == std::byte(0));
    REQUIRE(get<std::byte>(test[6]) == std::byte(0));
    REQUIRE(get<std::byte>(test[7]) == std::byte(1));
    REQUIRE(get<std::byte>(test[8]) == std::byte(1));
    REQUIRE(get<std::byte>(test[9]) == std::byte(0));
    REQUIRE(get<std::byte>(test[10]) == std::byte(1));
    REQUIRE(get<std::byte>(test[11]) == std::byte(0));
    REQUIRE(get<std::byte>(test[12]) == std::byte(0));
    REQUIRE(get<std::byte>(test[13]) == std::byte(0));
    REQUIRE(get<std::byte>(test[14]) == std::byte(0));
    REQUIRE(get<std::byte>(test[15]) == std::byte(1));
    REQUIRE(get<std::byte>(test[16]) == std::byte(1));
    REQUIRE(get<std::byte>(test[17]) == std::byte(1));
    REQUIRE(get<std::byte>(test[18]) == std::byte(0));
    REQUIRE(get<std::byte>(test[19]) == std::byte(0));
    REQUIRE(get<std::byte>(test[20]) == std::byte(0));
    REQUIRE(get<std::byte>(test[21]) == std::byte(1));
    REQUIRE(get<std::byte>(test[22]) == std::byte(1));
    REQUIRE(get<std::byte>(test[23]) == std::byte(1));
    REQUIRE(get<std::byte>(test[24]) == std::byte(1));
    REQUIRE(get<std::byte>(test[25]) == std::byte(1));
    REQUIRE(get<std::byte>(test[26]) == std::byte(1));
    REQUIRE(get<std::byte>(test[27]) == std::byte(0));
    REQUIRE(get<std::byte>(test[28]) == std::byte(0));
    REQUIRE(get<std::byte>(test[29]) == std::byte(1));
}
