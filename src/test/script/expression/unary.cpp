#include <catch2/catch_test_macros.hpp>

#include "script/expression/unary.hpp"
#include "script/script.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("UnaryPlus expressions", "[script]") {
    auto script = Script("");
    auto factor1 = syntax::Factor(syntax::Constant(3L, 1, 2), 1, 2);
    auto factor2 = syntax::Factor(syntax::Constant(4.0, 1, 2), 1, 2);
    auto factor3 = syntax::Factor(syntax::Constant("foo", 1, 2), 1, 2);
    auto test1 = UnaryPlus::from_ast(script, factor1);
    auto test2 = UnaryPlus::from_ast(script, factor2);
    REQUIRE_THROWS(UnaryPlus::from_ast(script, factor3));

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 3L);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 4.0);
    REQUIRE(test2->line_number() == 1);
    REQUIRE(test2->column_number() == 2);
}

TEST_CASE("UnaryMinus expressions", "[script]") {
    auto script = Script("");
    auto factor1 = syntax::Factor(syntax::Constant(3L, 1, 2), 1, 2);
    auto factor2 = syntax::Factor(syntax::Constant(4.0, 1, 2), 1, 2);
    auto factor3 = syntax::Factor(syntax::Constant("foo", 1, 2), 1, 2);
    auto test1 = UnaryMinus::from_ast(script, factor1);
    auto test2 = UnaryMinus::from_ast(script, factor2);
    REQUIRE_THROWS(UnaryMinus::from_ast(script, factor3));

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == -3L);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == -4.0);
    REQUIRE(test2->line_number() == 1);
    REQUIRE(test2->column_number() == 2);
}
