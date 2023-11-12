#include <catch2/catch_test_macros.hpp>

#include "ganim/color.hpp"

using namespace ganim;

TEST_CASE("Color numeric constructors", "[ganim]") {
    auto c = Color();
    REQUIRE(int(c.r) == 0);
    REQUIRE(int(c.g) == 0);
    REQUIRE(int(c.b) == 0);
    REQUIRE(int(c.a) == 0);

    c = Color(10, 20, 30, 40);
    REQUIRE(int(c.r) == 10);
    REQUIRE(int(c.g) == 20);
    REQUIRE(int(c.b) == 30);
    REQUIRE(int(c.a) == 40);

    c = Color(1.0, 0.5, 0.0, 2.0);
    REQUIRE(int(c.r) == 255);
    REQUIRE(int(c.g) == 127);
    REQUIRE(int(c.b) == 0);
    REQUIRE(int(c.a) == 255);
}

TEST_CASE("Color output", "[ganim]") {
    auto stream = std::stringstream();
    stream << Color(15, 31, 47, 255);
    REQUIRE(stream.str() == "#0F1F2F");
    stream.str("");
    stream << Color(15, 31, 47, 64);
    REQUIRE(stream.str() == "#0F1F2F40");
}

TEST_CASE("Color hex constructors", "[ganim]") {
    auto s1 = std::string("#0f1F2F");
    auto s2 = std::string_view("#0f1F2F40");
    auto s3 = "0f1F2F";
    auto s4 = "0f1F2F40";
    REQUIRE(Color(s1) == Color(15, 31, 47, 255));
    REQUIRE(Color(s2) == Color(15, 31, 47, 64));
    REQUIRE(Color(s3) == Color(15, 31, 47, 255));
    REQUIRE(Color(s4) == Color(15, 31, 47, 64));
    REQUIRE_THROWS(Color("#AAAAA"));
    REQUIRE_THROWS(Color("#AAAAAAA"));
    REQUIRE_THROWS(Color("#AAAAAAAAA"));
    REQUIRE_THROWS(Color("#ZZZZZZ"));
}
