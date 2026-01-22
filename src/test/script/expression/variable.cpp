#include <catch2/catch_test_macros.hpp>

#include "script/expression/variable.hpp"
#include "script/script.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("Variable from ast", "[script]") {
    auto script = Script("var a = 5;");
    script.compile();
    script.execute();
    auto identifier = syntax::IdentifierExpression("a", 1, 2);
    auto test = Variable::from_ast(script, identifier);
    REQUIRE(test->value().get_as<std::int64_t>());
    REQUIRE(*test->value().get_as<std::int64_t>() == 5);
    REQUIRE(test->line_number() == 1);
    REQUIRE(test->column_number() == 2);
}
