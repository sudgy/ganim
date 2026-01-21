#include <catch2/catch_test_macros.hpp>

#include "script/expression/variable.hpp"
#include "script/script.hpp"

using namespace ganim;

TEST_CASE("Expression from ast", "[script]") {
    auto script = Script("var foo = 2.0;");
    script.compile();
    script.execute();
    auto constant = syntax::Constant(5L, 1, 2);
    auto identifier = syntax::Identifier("foo", 2, 3);
    auto expression = std::make_unique<syntax::Expression>(
        nullptr,
        syntax::Term(
            nullptr,
            syntax::Factor(
                syntax::Constant(6L, 3, 4)
            ),
            3,
            4
        ),
        3,
        4
    );
    auto factor1 = syntax::Factor(constant, 1, 2);
    auto factor2 = syntax::Factor(identifier, 2, 3);
    auto factor3 = syntax::Factor(std::move(expression), 3, 4);
    auto test1 = Expression::from_ast(script, factor1);
    auto test2 = Expression::from_ast(script, factor2);
    auto test3 = Expression::from_ast(script, factor3);

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 5L);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 2.0);
    REQUIRE(test2->line_number() == 2);
    REQUIRE(test2->column_number() == 3);

    REQUIRE(test3->value().get_as<std::int64_t>());
    REQUIRE(*test3->value().get_as<std::int64_t>() == 6L);
    REQUIRE(test3->line_number() == 3);
    REQUIRE(test3->column_number() == 4);
}
