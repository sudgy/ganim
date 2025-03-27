#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/tokenize.hpp"

#include "script/script_exception.hpp"

using namespace ganim;

TEST_CASE("Tokenize", "[script]") {
    auto tokens1 = tokenize("a b c");
    auto tokens2 = tokenize("abc");
    auto tokens3 = tokenize("ab c");
    auto tokens4 = tokenize("  ab c");
    auto tokens5 = tokenize("  ab \n c");
    auto tokens6 = tokenize(" [  ab \n c");
    auto tokens7 = tokenize(" [ab \n c");
    auto tokens8 = tokenize(" [ab[ \n c");
    auto tokens9 = tokenize(" [ab· \n c");
    auto tokens10 = tokenize("a\nb");

    REQUIRE(tokens1.size() == 3);
    REQUIRE(tokens1[0].string == "a");
    REQUIRE(tokens1[0].line_number == 0);
    REQUIRE(tokens1[0].column_number == 0);
    REQUIRE(tokens1[0].byte_number == 0);
    REQUIRE(tokens1[1].string == "b");
    REQUIRE(tokens1[1].line_number == 0);
    REQUIRE(tokens1[1].column_number == 2);
    REQUIRE(tokens1[1].byte_number == 2);
    REQUIRE(tokens1[2].string == "c");
    REQUIRE(tokens1[2].line_number == 0);
    REQUIRE(tokens1[2].column_number == 4);
    REQUIRE(tokens1[2].byte_number == 4);

    REQUIRE(tokens2.size() == 1);
    REQUIRE(tokens2[0].string == "abc");
    REQUIRE(tokens2[0].line_number == 0);
    REQUIRE(tokens2[0].column_number == 0);
    REQUIRE(tokens2[0].byte_number == 0);

    REQUIRE(tokens3.size() == 2);
    REQUIRE(tokens3[0].string == "ab");
    REQUIRE(tokens3[0].line_number == 0);
    REQUIRE(tokens3[0].column_number == 0);
    REQUIRE(tokens3[0].byte_number == 0);
    REQUIRE(tokens3[1].string == "c");
    REQUIRE(tokens3[1].line_number == 0);
    REQUIRE(tokens3[1].column_number == 3);
    REQUIRE(tokens3[1].byte_number == 3);

    REQUIRE(tokens4.size() == 2);
    REQUIRE(tokens4[0].string == "ab");
    REQUIRE(tokens4[0].line_number == 0);
    REQUIRE(tokens4[0].column_number == 2);
    REQUIRE(tokens4[0].byte_number == 2);
    REQUIRE(tokens4[1].string == "c");
    REQUIRE(tokens4[1].line_number == 0);
    REQUIRE(tokens4[1].column_number == 5);
    REQUIRE(tokens4[1].byte_number == 5);

    REQUIRE(tokens5.size() == 2);
    REQUIRE(tokens5[0].string == "ab");
    REQUIRE(tokens5[0].line_number == 0);
    REQUIRE(tokens5[0].column_number == 2);
    REQUIRE(tokens5[0].byte_number == 2);
    REQUIRE(tokens5[1].string == "c");
    REQUIRE(tokens5[1].line_number == 1);
    REQUIRE(tokens5[1].column_number == 1);
    REQUIRE(tokens5[1].byte_number == 7);

    REQUIRE(tokens6.size() == 3);
    REQUIRE(tokens6[0].string == "[");
    REQUIRE(tokens6[0].line_number == 0);
    REQUIRE(tokens6[0].column_number == 1);
    REQUIRE(tokens6[0].byte_number == 1);
    REQUIRE(tokens6[1].string == "ab");
    REQUIRE(tokens6[1].line_number == 0);
    REQUIRE(tokens6[1].column_number == 4);
    REQUIRE(tokens6[1].byte_number == 4);
    REQUIRE(tokens6[2].string == "c");
    REQUIRE(tokens6[2].line_number == 1);
    REQUIRE(tokens6[2].column_number == 1);
    REQUIRE(tokens6[2].byte_number == 9);

    REQUIRE(tokens7.size() == 3);
    REQUIRE(tokens7[0].string == "[");
    REQUIRE(tokens7[0].line_number == 0);
    REQUIRE(tokens7[0].column_number == 1);
    REQUIRE(tokens7[0].byte_number == 1);
    REQUIRE(tokens7[1].string == "ab");
    REQUIRE(tokens7[1].line_number == 0);
    REQUIRE(tokens7[1].column_number == 2);
    REQUIRE(tokens7[1].byte_number == 2);
    REQUIRE(tokens7[2].string == "c");
    REQUIRE(tokens7[2].line_number == 1);
    REQUIRE(tokens7[2].column_number == 1);
    REQUIRE(tokens7[2].byte_number == 7);

    REQUIRE(tokens8.size() == 4);
    REQUIRE(tokens8[0].string == "[");
    REQUIRE(tokens8[0].line_number == 0);
    REQUIRE(tokens8[0].column_number == 1);
    REQUIRE(tokens8[0].byte_number == 1);
    REQUIRE(tokens8[1].string == "ab");
    REQUIRE(tokens8[1].line_number == 0);
    REQUIRE(tokens8[1].column_number == 2);
    REQUIRE(tokens8[1].byte_number == 2);
    REQUIRE(tokens8[2].string == "[");
    REQUIRE(tokens8[2].line_number == 0);
    REQUIRE(tokens8[2].column_number == 4);
    REQUIRE(tokens8[2].byte_number == 4);
    REQUIRE(tokens8[3].string == "c");
    REQUIRE(tokens8[3].line_number == 1);
    REQUIRE(tokens8[3].column_number == 1);
    REQUIRE(tokens8[3].byte_number == 8);

    REQUIRE(tokens9.size() == 4);
    REQUIRE(tokens9[0].string == "[");
    REQUIRE(tokens9[0].line_number == 0);
    REQUIRE(tokens9[0].column_number == 1);
    REQUIRE(tokens9[0].byte_number == 1);
    REQUIRE(tokens9[1].string == "ab");
    REQUIRE(tokens9[1].line_number == 0);
    REQUIRE(tokens9[1].column_number == 2);
    REQUIRE(tokens9[1].byte_number == 2);
    REQUIRE(tokens9[2].string == "·");
    REQUIRE(tokens9[2].line_number == 0);
    REQUIRE(tokens9[2].column_number == 4);
    REQUIRE(tokens9[2].byte_number == 4);
    REQUIRE(tokens9[3].string == "c");
    REQUIRE(tokens9[3].line_number == 1);
    REQUIRE(tokens9[3].column_number == 1);
    REQUIRE(tokens9[3].byte_number == 9);

    REQUIRE(tokens10.size() == 2);
    REQUIRE(tokens10[0].string == "a");
    REQUIRE(tokens10[0].line_number == 0);
    REQUIRE(tokens10[0].column_number == 0);
    REQUIRE(tokens10[0].byte_number == 0);
    REQUIRE(tokens10[1].string == "b");
    REQUIRE(tokens10[1].line_number == 1);
    REQUIRE(tokens10[1].column_number == 0);
    REQUIRE(tokens10[1].byte_number == 2);
}

TEST_CASE("tokenize with strings", "[script]") {
    auto tokens1 = tokenize("a \"b\" c");
    auto tokens2 = tokenize("ab\"c\"");
    auto tokens3 = tokenize("ab\"c\n\"");
    auto tokens4 = tokenize("ab\"c\nd\" e");

    REQUIRE(tokens1.size() == 3);
    REQUIRE(tokens1[0].string == "a");
    REQUIRE(tokens1[0].line_number == 0);
    REQUIRE(tokens1[0].column_number == 0);
    REQUIRE(tokens1[0].byte_number == 0);
    REQUIRE(tokens1[1].string == "\"b\"");
    REQUIRE(tokens1[1].line_number == 0);
    REQUIRE(tokens1[1].column_number == 2);
    REQUIRE(tokens1[1].byte_number == 2);
    REQUIRE(tokens1[2].string == "c");
    REQUIRE(tokens1[2].line_number == 0);
    REQUIRE(tokens1[2].column_number == 6);
    REQUIRE(tokens1[2].byte_number == 6);

    REQUIRE(tokens2.size() == 2);
    REQUIRE(tokens2[0].string == "ab");
    REQUIRE(tokens2[0].line_number == 0);
    REQUIRE(tokens2[0].column_number == 0);
    REQUIRE(tokens2[0].byte_number == 0);
    REQUIRE(tokens2[1].string == "\"c\"");
    REQUIRE(tokens2[1].line_number == 0);
    REQUIRE(tokens2[1].column_number == 2);
    REQUIRE(tokens2[1].byte_number == 2);

    REQUIRE(tokens3.size() == 2);
    REQUIRE(tokens3[0].string == "ab");
    REQUIRE(tokens3[0].line_number == 0);
    REQUIRE(tokens3[0].column_number == 0);
    REQUIRE(tokens3[0].byte_number == 0);
    REQUIRE(tokens3[1].string == "\"c\n\"");
    REQUIRE(tokens3[1].line_number == 0);
    REQUIRE(tokens3[1].column_number == 2);
    REQUIRE(tokens3[1].byte_number == 2);

    REQUIRE(tokens4.size() == 3);
    REQUIRE(tokens4[0].string == "ab");
    REQUIRE(tokens4[0].line_number == 0);
    REQUIRE(tokens4[0].column_number == 0);
    REQUIRE(tokens4[0].byte_number == 0);
    REQUIRE(tokens4[1].string == "\"c\nd\"");
    REQUIRE(tokens4[1].line_number == 0);
    REQUIRE(tokens4[1].column_number == 2);
    REQUIRE(tokens4[1].byte_number == 2);
    REQUIRE(tokens4[2].string == "e");
    REQUIRE(tokens4[2].line_number == 1);
    REQUIRE(tokens4[2].column_number == 3);
    REQUIRE(tokens4[2].byte_number == 8);

    // Using wide numeric characters so that it doesn't start trying to read it
    // as a number
    REQUIRE_THROWS_WITH(tokenize("abc ab１ １ab"),
            get_compile_error_message(0, 8, "Invalid identifier"));
}

TEST_CASE("tokenize numbers", "[script]") {
    auto tokens1 = tokenize("123 456 a23a");

    REQUIRE(tokens1.size() == 3);
    REQUIRE(tokens1[0].string == "123");
    REQUIRE(tokens1[0].line_number == 0);
    REQUIRE(tokens1[0].column_number == 0);
    REQUIRE(tokens1[0].byte_number == 0);
    REQUIRE(tokens1[1].string == "456");
    REQUIRE(tokens1[1].line_number == 0);
    REQUIRE(tokens1[1].column_number == 4);
    REQUIRE(tokens1[1].byte_number == 4);
    REQUIRE(tokens1[2].string == "a23a");
    REQUIRE(tokens1[2].line_number == 0);
    REQUIRE(tokens1[2].column_number == 8);
    REQUIRE(tokens1[2].byte_number == 8);

    REQUIRE_THROWS_WITH(tokenize("ab 1ab"),
            get_compile_error_message(0, 3, "Invalid numeric literal"));
}
