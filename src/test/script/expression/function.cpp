#include <catch2/catch_test_macros.hpp>

#include "script/expression/function.hpp"
#include "script/script.hpp"

using namespace ganim;
using namespace ganim::expressions;

TEST_CASE("Binary expressions", "[script]") {
    auto script = Script(R"(
var a = 3 + 4
var b = 5.0 + 6.0
var c = "foo" + "bar"
var d = 3 * 4
var e = 5.0 * 6.0
var f = 4 / 3
var g = 5.0 / 2.0
var h = 2 - 3
var i = 3.0 - 5.0
var j = 5 % 2
    )");
    script.compile();
    script.execute();
    auto bad_script1 = Script("var a = true + false");
    auto bad_script2 = Script("var a = 3 + \"foo\"");
    auto bad_script3 = Script("var a = \"foo\" * \"bar\"");
    auto bad_script4 = Script("var a = 3.0 % 2.0");
    REQUIRE_THROWS(bad_script1.compile());
    REQUIRE_THROWS(bad_script2.compile());
    REQUIRE_THROWS(bad_script3.compile());
    REQUIRE_THROWS(bad_script4.compile());

    auto test1 = script.get_variable("a");
    auto test2 = script.get_variable("b");
    auto test3 = script.get_variable("c");
    auto test4 = script.get_variable("d");
    auto test5 = script.get_variable("e");
    auto test6 = script.get_variable("f");
    auto test7 = script.get_variable("g");
    auto test8 = script.get_variable("h");
    auto test9 = script.get_variable("i");
    auto test10 = script.get_variable("j");

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 7);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 11.0);

    REQUIRE(test3->value().get_as<std::string>());
    REQUIRE(*test3->value().get_as<std::string>() == "foobar");

    REQUIRE(test4->value().get_as<std::int64_t>());
    REQUIRE(*test4->value().get_as<std::int64_t>() == 12);

    REQUIRE(test5->value().get_as<double>());
    REQUIRE(*test5->value().get_as<double>() == 30.0);

    REQUIRE(test6->value().get_as<std::int64_t>());
    REQUIRE(*test6->value().get_as<std::int64_t>() == 1);

    REQUIRE(test7->value().get_as<double>());
    REQUIRE(*test7->value().get_as<double>() == 2.5);

    REQUIRE(test8->value().get_as<std::int64_t>());
    REQUIRE(*test8->value().get_as<std::int64_t>() == -1);

    REQUIRE(test9->value().get_as<double>());
    REQUIRE(*test9->value().get_as<double>() == -2.0);

    REQUIRE(test10->value().get_as<std::int64_t>());
    REQUIRE(*test10->value().get_as<std::int64_t>() == 1);
}

TEST_CASE("UnaryPlus expressions", "[script]") {
    auto script = Script(R"(
var a = +3
var b = +4.0
    )");
    script.compile();
    script.execute();
    auto bad_script = Script("var a = +\"foo\"");
    REQUIRE_THROWS(bad_script.compile());

    auto test1 = script.get_variable("a");
    auto test2 = script.get_variable("b");

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == 3L);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == 4.0);
}

TEST_CASE("UnaryMinus expressions", "[script]") {
    auto script = Script(R"(
var a = -3
var b = -4.0
    )");
    script.compile();
    script.execute();
    auto bad_script = Script("var a = -\"foo\"");
    REQUIRE_THROWS(bad_script.compile());

    auto test1 = script.get_variable("a");
    auto test2 = script.get_variable("b");

    REQUIRE(test1->value().get_as<std::int64_t>());
    REQUIRE(*test1->value().get_as<std::int64_t>() == -3L);

    REQUIRE(test2->value().get_as<double>());
    REQUIRE(*test2->value().get_as<double>() == -4.0);
}
