#include <catch2/catch_test_macros.hpp>

#include "sample_parser.hpp"

TEST_CASE("Parser sample grammar", "[parse]") {
    auto parser = sample::parser();
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::Constant_token(3.0)));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::builtin_token("+")));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::Constant_token(2.0)));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::builtin_token("*")));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::builtin_token("(")));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::Constant_token(4.0)));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::builtin_token("+")));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::Constant_token(5.0)));
    REQUIRE(!parser.finished());
    REQUIRE(parser.push(sample::parser::builtin_token(")")));
    REQUIRE(!parser.finished());
    REQUIRE(!parser.push(sample::parser::end_token()));
    REQUIRE(parser.finished());
    REQUIRE(parser.take_result() == 21.0);

    auto parser2 = sample::parser();
    REQUIRE_THROWS(parser2.push(sample::parser::builtin_token("+")));

    auto parser3 = sample::parser();
    REQUIRE(parser3.push(sample::parser::Constant_token(1.0)));
    REQUIRE(!parser3.finished());
    REQUIRE(!parser3.push(sample::parser::builtin_token("(")));
    REQUIRE(parser3.finished());
    REQUIRE(parser3.take_result() == 1.0);
}
