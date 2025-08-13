#include <catch2/catch_test_macros.hpp>

#include "gex_dimension_parser.hpp"

using namespace ganim::gex;

TEST_CASE("GeX Dimension Parser", "[object][text][gex]") {
    auto parser = DimensionParser();
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('4')));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('3')));
    REQUIRE(parser.push(DimensionParser::DecimalDigit_token('9')));
    REQUIRE(parser.push(DimensionParser::builtin_token("s")));
    REQUIRE(parser.push(DimensionParser::builtin_token("p")));
    REQUIRE(!parser.push(DimensionParser::builtin_token("t")));
    REQUIRE(parser.finished());
    REQUIRE(parser.get_result() == 439.0*65536.0);
}

TEST_CASE("GeX Dimension Parser decimals", "[object][text][gex]") {
    auto parser = DimensionParser();
    REQUIRE(parser.push(DimensionParser::builtin_token("-")));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('2')));
    REQUIRE(parser.push(DimensionParser::builtin_token(".")));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('2')));
    REQUIRE(parser.push(DimensionParser::builtin_token("p")));
    REQUIRE(parser.push(DimensionParser::builtin_token("t")));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(!parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.get_result() == -2.2);
}

TEST_CASE("GeX Dimension Parser weird sign stuff", "[object][text][gex]") {
    auto parser = DimensionParser();
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::builtin_token("-")));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::builtin_token("+")));
    REQUIRE(parser.push(DimensionParser::builtin_token("-")));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::builtin_token("-")));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::Space_token(' ')));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('2')));
    REQUIRE(parser.push(DimensionParser::builtin_token("p")));
    REQUIRE(parser.push(DimensionParser::builtin_token("t")));
    REQUIRE(!parser.push(DimensionParser::end_token()));
    REQUIRE(parser.get_result() == -2);
}

TEST_CASE("GeX Dimension Parser weird looking decimals", "[object][text][gex]") {
    auto parser = DimensionParser();
    REQUIRE(parser.push(DimensionParser::builtin_token("-")));
    REQUIRE(parser.push(DimensionParser::builtin_token(".")));
    REQUIRE(parser.push(DimensionParser::builtin_token("p")));
    REQUIRE(parser.push(DimensionParser::builtin_token("t")));
    REQUIRE(!parser.push(DimensionParser::end_token()));
    REQUIRE(parser.get_result() == 0);
}

TEST_CASE("GeX Dimension Parser octal", "[object][text][gex]") {
    auto parser = DimensionParser();
    REQUIRE(parser.push(DimensionParser::builtin_token("'")));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('1')));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('0')));
    REQUIRE(parser.push(DimensionParser::builtin_token("p")));
    REQUIRE(parser.push(DimensionParser::builtin_token("t")));
    REQUIRE(!parser.push(DimensionParser::end_token()));
    REQUIRE(parser.finished());
    REQUIRE(parser.get_result() == 8);
}

TEST_CASE("GeX Dimension Parser hexadecimal", "[object][text][gex]") {
    auto parser = DimensionParser();
    REQUIRE(parser.push(DimensionParser::builtin_token("\"")));
    REQUIRE(parser.push(DimensionParser::OctalDigit_token('1')));
    REQUIRE(parser.push(DimensionParser::HexDigit_token('F')));
    REQUIRE(parser.push(DimensionParser::builtin_token("p")));
    REQUIRE(parser.push(DimensionParser::builtin_token("t")));
    REQUIRE(!parser.push(DimensionParser::end_token()));
    REQUIRE(parser.finished());
    REQUIRE(parser.get_result() == 31);
}
