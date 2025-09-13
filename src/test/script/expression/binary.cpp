#include <catch2/catch_test_macros.hpp>

#include "script/expression/binary.hpp"
#include "script/script.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("Sum expressions", "[script]") {
    auto script = Script("");
    // Expression 1: 3 + 4
    // Expression 2: 5.0 + 6.0
    // Expression 3: "foo" + "bar"
    // Expression 4: true + false
    // Expression 5: 3 + "foo"
    auto expression1 = syntax::Expression(
        std::make_unique<syntax::Expression>(
            nullptr,
            syntax::Term(
                nullptr,
                syntax::Factor(
                    syntax::Constant(3L, 1, 2),
                    1, 2
                ),
                1, 2
            ),
            1, 2
        ),
        syntax::Term(
            nullptr,
            syntax::Factor(
                syntax::Constant(4L, 1, 4),
                1, 4
            ),
            1, 4
        ),
        1, 2
    );
    auto expression2 = syntax::Expression(
        std::make_unique<syntax::Expression>(
            nullptr,
            syntax::Term(
                nullptr,
                syntax::Factor(
                    syntax::Constant(5.0, 1, 2),
                    1, 2
                ),
                1, 2
            ),
            1, 2
        ),
        syntax::Term(
            nullptr,
            syntax::Factor(
                syntax::Constant(6.0, 1, 4),
                1, 4
            ),
            1, 4
        ),
        1, 2
    );
    auto expression3 = syntax::Expression(
        std::make_unique<syntax::Expression>(
            nullptr,
            syntax::Term(
                nullptr,
                syntax::Factor(
                    syntax::Constant("foo", 1, 2),
                    1, 2
                ),
                1, 2
            ),
            1, 2
        ),
        syntax::Term(
            nullptr,
            syntax::Factor(
                syntax::Constant("bar", 1, 4),
                1, 4
            ),
            1, 4
        ),
        1, 2
    );
    auto expression4 = syntax::Expression(
        std::make_unique<syntax::Expression>(
            nullptr,
            syntax::Term(
                nullptr,
                syntax::Factor(
                    syntax::Constant(true, 1, 2),
                    1, 2
                ),
                1, 2
            ),
            1, 2
        ),
        syntax::Term(
            nullptr,
            syntax::Factor(
                syntax::Constant(false, 1, 4),
                1, 4
            ),
            1, 4
        ),
        1, 2
    );
    auto expression5 = syntax::Expression(
        std::make_unique<syntax::Expression>(
            nullptr,
            syntax::Term(
                nullptr,
                syntax::Factor(
                    syntax::Constant(3L, 1, 2),
                    1, 2
                ),
                1, 2
            ),
            1, 2
        ),
        syntax::Term(
            nullptr,
            syntax::Factor(
                syntax::Constant("foo", 1, 4),
                1, 4
            ),
            1, 4
        ),
        1, 2
    );

    auto test1 = Sum::from_ast(script, expression1);
    auto test2 = Sum::from_ast(script, expression2);
    auto test3 = Sum::from_ast(script, expression3);
    REQUIRE_THROWS(Sum::from_ast(script, expression4));
    REQUIRE_THROWS(Sum::from_ast(script, expression5));

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 7);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 11.0);
    REQUIRE(test2->line_number() == 1);
    REQUIRE(test2->column_number() == 2);

    REQUIRE(test3->value().get_as<std::string>());
    REQUIRE(*test3->value().get_as<std::string>() == "foobar");
    REQUIRE(test3->line_number() == 1);
    REQUIRE(test3->column_number() == 2);
}

TEST_CASE("Product expressions", "[script]") {
    auto script = Script("");
    // Expression 1: 3 * 4
    // Expression 2: 5.0 * 6.0
    // Expression 3: "foo" * "bar"
    // Expression 4: true * false
    // Expression 5: 3 * "foo"
    auto term1 = syntax::Term(
        std::make_unique<syntax::Term>(
            nullptr,
            syntax::Factor(
                syntax::Constant(3L, 1, 2),
                1, 2
            ),
            1, 2
        ),
        syntax::Factor(
            syntax::Constant(4L, 1, 4),
            1, 4
        ),
        1, 2
    );
    auto term2 = syntax::Term(
        std::make_unique<syntax::Term>(
            nullptr,
            syntax::Factor(
                syntax::Constant(5.0, 1, 2),
                1, 2
            ),
            1, 2
        ),
        syntax::Factor(
            syntax::Constant(6.0, 1, 4),
            1, 4
        ),
        1, 2
    );
    auto term3 = syntax::Term(
        std::make_unique<syntax::Term>(
            nullptr,
            syntax::Factor(
                syntax::Constant("foo", 1, 2),
                1, 2
            ),
            1, 2
        ),
        syntax::Factor(
            syntax::Constant("bar", 1, 4),
            1, 4
        ),
        1, 2
    );
    auto term4 = syntax::Term(
        std::make_unique<syntax::Term>(
            nullptr,
            syntax::Factor(
                syntax::Constant(true, 1, 2),
                1, 2
            ),
            1, 2
        ),
        syntax::Factor(
            syntax::Constant(false, 1, 4),
            1, 4
        ),
        1, 2
    );
    auto term5 = syntax::Term(
        std::make_unique<syntax::Term>(
            nullptr,
            syntax::Factor(
                syntax::Constant(3L, 1, 2),
                1, 2
            ),
            1, 2
        ),
        syntax::Factor(
            syntax::Constant("foo", 1, 4),
            1, 4
        ),
        1, 2
    );

    auto test1 = Product::from_ast(script, term1);
    auto test2 = Product::from_ast(script, term2);
    REQUIRE_THROWS(Product::from_ast(script, term3));
    REQUIRE_THROWS(Product::from_ast(script, term4));
    REQUIRE_THROWS(Product::from_ast(script, term5));

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 12);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 30.0);
    REQUIRE(test2->line_number() == 1);
    REQUIRE(test2->column_number() == 2);
}
