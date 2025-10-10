#include <catch2/catch_test_macros.hpp>

#include "script/expression/constant.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("Constant", "[script]") {
    auto expr = Constant(1, 2, 3);
    REQUIRE(expr.type_id() == any_pointer::get_tag<int>());
    REQUIRE(expr.value());
    REQUIRE(expr.value().get_as<int>());
    REQUIRE(*expr.value().get_as<int>() == 1);
    REQUIRE(expr.line_number() == 2);
    REQUIRE(expr.column_number() == 3);
}

TEST_CASE("Constant from ast", "[script]") {
    auto int_ast = syntax::Constant(5L, 1, 2);
    auto double_ast = syntax::Constant(3.0, 2, 3);
    auto bool_ast = syntax::Constant(true, 3, 4);
    auto string_ast = syntax::Constant("test", 4, 5);
    auto test1 = constant_from_ast(int_ast);
    auto test2 = constant_from_ast(double_ast);
    auto test3 = constant_from_ast(bool_ast);
    auto test4 = constant_from_ast(string_ast);

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 5);
    REQUIRE(test1->line_number() == 1);
    REQUIRE(test1->column_number() == 2);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 3.0);
    REQUIRE(test2->line_number() == 2);
    REQUIRE(test2->column_number() == 3);

    REQUIRE(test3->value().get_as<bool>());
    REQUIRE(*test3->value().get_as<bool>() == true);
    REQUIRE(test3->line_number() == 3);
    REQUIRE(test3->column_number() == 4);

    REQUIRE(test4->value().get_as<std::string>());
    REQUIRE(*test4->value().get_as<std::string>() == "test");
    REQUIRE(test4->line_number() == 4);
    REQUIRE(test4->column_number() == 5);
}
