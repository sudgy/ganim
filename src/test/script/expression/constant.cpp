#include <catch2/catch_test_macros.hpp>

#include "script/expression/constant.hpp"

#include "script/script.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("Constant", "[script]") {
    auto expr = Constant(1, 2, 3);
    REQUIRE(expr.type() == Type{any_pointer::get_tag<int>()});
    REQUIRE(expr.value());
    REQUIRE(expr.value().get_as<int>());
    REQUIRE(*expr.value().get_as<int>() == 1);
    REQUIRE(expr.line_number() == 2);
    REQUIRE(expr.column_number() == 3);
}

TEST_CASE("Constant from ast", "[script]") {
    auto int_ast = syntax::ConstantExpression(5L, 1, 2);
    auto double_ast = syntax::ConstantExpression(3.0, 2, 3);
    auto bool_ast = syntax::ConstantExpression(true, 3, 4);
    auto string_ast = syntax::ConstantExpression("test", 4, 5);
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

TEST_CASE("Numeric expressions", "[script]") {
    auto script = Script(R"(
var a = 523;
var b = 0xFF;
var c = 010;
var d = 0b11;
var e = 0.5;
var f = 0;
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    auto b = script.symbol_table().get_variable("b");
    auto c = script.symbol_table().get_variable("c");
    auto d = script.symbol_table().get_variable("d");
    auto e = script.symbol_table().get_variable("e");
    auto f = script.symbol_table().get_variable("f");
    REQUIRE( a->value().get_as<std::int64_t>());
    REQUIRE(*a->value().get_as<std::int64_t>() == 523);
    REQUIRE( b->value().get_as<std::int64_t>());
    REQUIRE(*b->value().get_as<std::int64_t>() == 0xFF);
    REQUIRE( c->value().get_as<std::int64_t>());
    REQUIRE(*c->value().get_as<std::int64_t>() == 010);
    REQUIRE( d->value().get_as<std::int64_t>());
    REQUIRE(*d->value().get_as<std::int64_t>() == 0b11);
    REQUIRE( e->value().get_as<double>());
    REQUIRE(*e->value().get_as<double>() == 0.5);
    REQUIRE( f->value().get_as<std::int64_t>());
    REQUIRE(*f->value().get_as<std::int64_t>() == 0);
}

TEST_CASE("String expressions", "[script]") {
    auto script = Script(R"(
var a = "ab\"\\c";
    )");
    script.compile();
    script.execute();
    auto a = script.symbol_table().get_variable("a");
    REQUIRE( a->value().get_as<std::string>());
    REQUIRE(*a->value().get_as<std::string>() == "ab\"\\c");
}
