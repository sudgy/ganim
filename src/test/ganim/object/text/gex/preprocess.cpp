#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "ganim/object/text/gex/preprocess.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    TokenList get_tokens(
        std::string_view str,
        std::vector<int> groups,
        std::vector<int> indices
    )
    {
        auto result = TokenList();
        for (int i = 0; i < ssize(str); ++i) {
            auto catcode = CategoryCode::Letter;
            if (str[i] == ' ') catcode = CategoryCode::Spacer;
            if (str[i] == ',') catcode = CategoryCode::Other;
            if (str[i] == '{') catcode = CategoryCode::Other;
            if (str[i] == '%') catcode = CategoryCode::Other;
            result.emplace_back(
                CharacterToken(str[i], catcode),
                groups[i],
                indices[i]
            );
        }
        return result;
    }
    void test_tokens(const TokenList& tokens1, const TokenList& tokens2)
    {
        REQUIRE(tokens1.size() == tokens2.size());
        for (int i = 0; i < ssize(tokens1); ++i) {
            INFO("i = " << i);
            REQUIRE(tokens1[i].value == tokens2[i].value);
            REQUIRE(tokens1[i].group == tokens2[i].group);
            REQUIRE(tokens1[i].string_index == tokens2[i].string_index);
        }
    }
}

TEST_CASE("GeX Hello", "[object][text][gex]") {
    auto tokens1 = preprocess({"He", "llo"});
    auto tokens2 = get_tokens("Hello", {0, 0, 1, 1, 1}, {0, 1, 0, 1, 2});

    test_tokens(tokens1, tokens2);
}

TEST_CASE("Basic macros", "[object][text][gex]") {
    auto tokens1 = preprocess({"A", "\\{\\%", "B\\relax C"});
    auto tokens2 = get_tokens("A{%BC", {0, 1, 1, 2, 2}, {0, 0, 2, 0, 8});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess({"\\oopsy"}), "GeX compilation error in "
            "group 0 index 0: Undefined control sequence \"oopsy\"");
}

TEST_CASE("Defining basic macros", "[object][text][gex]") {
    auto tokens1 = preprocess({"a\\def\\aa{b}", "\\aa"});
    auto tokens2 = get_tokens("ab", {0, 1}, {0, 0});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess({"\\def"}), "GeX compilation error in group "
            "0 index 4: Expected control sequence");
    REQUIRE_THROWS_WITH(preprocess({"\\def aa"}), "GeX compilation error in "
            "group 0 index 5: Expected control sequence");
    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa"}), "GeX compilation error in "
            "group 0 index 7: End of input reached while processing a "
            "definition");
    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa}"}), "GeX compilation error in "
            "group 0 index 7: Unexpected end group token");
    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa{"}), "GeX compilation error in "
            "group 0 index 8: End of input reached while processing a "
            "definition");
}

TEST_CASE("Macro expansion order", "[object][text][gex]") {
    auto tokens1 = preprocess({"\\def\\aa{a\\def\\aa{b}}", "\\aa", "\\aa"});
    auto tokens2 = get_tokens("ab", {1, 2}, {0, 0});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa{\\def b}\\aa"}), "GeX "
        "compilation error in group 0 index 15: Error during macro expansion: "
        "Expected control sequence");
}

TEST_CASE("Macros with delimiters", "[object][text][gex]") {
    auto tokens1 = preprocess({"a  \\def\\aa |  |{b}", "\\aa| |"});
    auto tokens2 = get_tokens("a  b", {0, 0, 0, 1}, {0, 1, 2, 0});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa|{b}\\aa"}), "GeX compilation "
            "error in group 0 index 14: Input ended while processing macro "
            "\"aa\"");
    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa|{b}\\aa["}), "GeX compilation "
            "error in group 0 index 14: Use of \\aa does not match its "
            "definition");
}

TEST_CASE("Macros with parameters", "[object][text][gex]") {
    auto gex1_tokens = preprocess({R"(\def\aa#1{a#1}\aa b)"});
    auto gex2_tokens = preprocess({R"(\def\aa#1{a#1}\aa{b})"});
    auto gex3_tokens = preprocess({R"(\def\aa#1{a#1}\aa{bb})"});
    auto gex4_tokens = preprocess({R"(\def\aa#1#2{a#1b#2}\aa c d)"});
    auto gex5_tokens = preprocess({R"(\def\aa#1#2{a#1b#2}\aa{cc}{dd})"});
    auto gex6_tokens = preprocess({R"(\def\aa|#1 #2|{a#1b#2}\aa|cc dd|)"});
    auto gex7_tokens = preprocess({R"(\def\aa|#1#2|{a#1b#2}\aa|cc dd|)"});
    auto gex8_tokens = preprocess({R"(\def\aa|#1|{a#1b}\aa||)"});
    auto gex9_tokens = preprocess({R"(\def\aa#1#2{a#1b#2c}\aa)", "{d}", "{e}"});
    auto gex10_tokens = preprocess({R"(\def\aa#1,#2{a#1b#2}\aa{c,d},e)"});
    auto gex11_tokens = preprocess({
        R"(\def\aa#1{a#1a})"
        R"(\def\bb#1{\aa#1})"
        R"(\bb b )"
        R"(\bb bb )"
        R"(\bb{bb} )"
        R"(\bb{{bb}})"
    });
    auto gex12_tokens = preprocess({
        R"(\def\aa#1{a#1a})"
        R"(\def\bb#1|{\aa#1})"
        R"(\bb bb| )"
        R"(\bb{bb}| )"
        R"(\bb{bb}c|)"
    });

    auto tokens1 = get_tokens("ab", {0, 0}, {14, 18});
    auto tokens2 = tokens1;
    auto tokens3 = get_tokens("abb", {0, 0, 0}, {14, 18, 19});
    auto tokens4 = get_tokens("acbd", {0, 0, 0, 0}, {19, 23, 19, 25});
    auto tokens5 = get_tokens(
            "accbdd", {0, 0, 0, 0, 0, 0}, {19, 23, 24, 19, 27, 28});
    auto tokens6 = get_tokens(
            "accbdd", {0, 0, 0, 0, 0, 0}, {22, 26, 27, 22, 29, 30});
    auto tokens7 = get_tokens(
            "acbc dd", {0, 0, 0, 0, 0, 0, 0}, {21, 25, 21, 26, 27, 28, 29});
    auto tokens8 = get_tokens("ab", {0, 0}, {17, 17});
    auto tokens9 = get_tokens("adbec", {0, 1, 0, 2, 0}, {20, 1, 20, 1, 20});
    auto tokens10 = get_tokens(
            "ac,dbe", {0, 0, 0, 0, 0, 0}, {20, 24, 25, 26, 20, 29});
    auto tokens11 = get_tokens(
            "aba abab abab abba",
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {31, 35, 31, 36,
             37, 41, 37, 42, 43,
             44, 48, 44, 49, 51,
             52, 57, 58, 52});
    auto tokens12 = get_tokens(
            "abab abab abbac",
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {32, 36, 32, 37, 39,
             40, 44, 40, 45, 48,
             49, 53, 54, 49, 56}
    );

    test_tokens(gex1_tokens, tokens1);
    test_tokens(gex2_tokens, tokens2);
    test_tokens(gex3_tokens, tokens3);
    test_tokens(gex4_tokens, tokens4);
    test_tokens(gex5_tokens, tokens5);
    test_tokens(gex6_tokens, tokens6);
    test_tokens(gex7_tokens, tokens7);
    test_tokens(gex8_tokens, tokens8);
    test_tokens(gex9_tokens, tokens9);
    test_tokens(gex10_tokens, tokens10);
    test_tokens(gex11_tokens, tokens11);
    test_tokens(gex12_tokens, tokens12);

    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa#2#1{b}"}), "GeX compilation "
            "error in group 0 index 7: Parameters must be numbered "
            "consecutively");
    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa#1{#2}"}), "GeX compilation "
            "error in group 0 index 10: Illegal parameter number");
    REQUIRE_THROWS_WITH(preprocess({"\\def\\aa#1{#1}\\aa"}), "GeX compilation "
            "error in group 0 index 16: Input ended while processing macro "
            "\"aa\"");
}

TEST_CASE("preprocess style commands", "[object][text][gex]") {
    auto tokens = preprocess({"\\displaystyle", "\\scriptstyle"});
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].group == 0);
    REQUIRE(tokens[1].group == 1);
    REQUIRE(tokens[0].string_index == 0);
    REQUIRE(tokens[1].string_index == 0);
    auto& token1 = get<CommandToken>(tokens[0].value);
    auto& token2 = get<CommandToken>(tokens[1].value);
    REQUIRE(token1.command == U"displaystyle");
    REQUIRE(token1.command_utf8 == "displaystyle");
    REQUIRE(token2.command == U"scriptstyle");
    REQUIRE(token2.command_utf8 == "scriptstyle");
}
