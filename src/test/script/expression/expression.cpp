#include <catch2/catch_test_macros.hpp>

#include "script/expression/variable.hpp"
#include "script/script.hpp"

using namespace ganim;

TEST_CASE("Expression from ast", "[script]") {
    auto script = Script("var foo = 2.0;");
    script.compile();
    script.execute();
    auto constant = syntax::ConstantExpression(5L, 1, 2);
    auto identifier = syntax::IdentifierExpression("foo", 2, 3);
    auto factor1 = syntax::Expression(constant, 1, 2);
    auto factor2 = syntax::Expression(identifier, 2, 3);
    auto test1 = Expression::from_ast(script, factor1);
    auto test2 = Expression::from_ast(script, factor2);

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 5L);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 2.0);
    REQUIRE(test2->line_number() == 2);
    REQUIRE(test2->column_number() == 3);
}
