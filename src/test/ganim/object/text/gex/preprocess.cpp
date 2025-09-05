#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "ganim/object/text/gex/preprocess.hpp"
#include "ganim/object/text/gex/macro.hpp"

#include <unordered_map>

using namespace ganim;
using namespace ganim::gex;

namespace {
    auto catcode_map1 = std::unordered_map<char, CategoryCode>{
        {' ', CategoryCode::Spacer},
        {',', CategoryCode::Other},
        {'{', CategoryCode::Other},
        {'%', CategoryCode::Other}
    };
    auto catcode_map2 = std::unordered_map<char, CategoryCode>{
        {' ', CategoryCode::Other},
        {'{', CategoryCode::StartGroup},
        {'}', CategoryCode::EndGroup}
    };
    TokenList get_tokens(
        std::string_view str,
        std::vector<int> groups,
        std::vector<int> indices,
        std::unordered_map<char, CategoryCode> catcode_map = catcode_map1
    )
    {
        auto result = TokenList();
        for (int i = 0; i < ssize(str); ++i) {
            auto catcode = CategoryCode::Letter;
            auto it = catcode_map.find(str[i]);
            if (it != catcode_map.end()) {
                catcode = it->second;
            }
            result.emplace_back(
                CharacterToken(str[i], catcode),
                groups[i],
                indices[i]
            );
        }
        return result;
    }
    TokenList get_tokens(
        std::string_view str,
        int glyphs_size,
        std::unordered_map<char, CategoryCode> catcode_map = catcode_map1
    )
    {
        auto groups = std::vector<int>();
        auto indices = std::vector<int>();
        groups.resize(glyphs_size, -1);
        indices.resize(glyphs_size, -1);
        return get_tokens(str, groups, indices, catcode_map);
    }
    void test_tokens(const TokenList& tokens1, const TokenList& tokens2)
    {
        REQUIRE(tokens1.size() == tokens2.size());
        for (int i = 0; i < ssize(tokens1); ++i) {
            INFO("i = " << i);
            REQUIRE(tokens1[i].value == tokens2[i].value);
            if (tokens2[i].group != -1) {
                REQUIRE(tokens1[i].group == tokens2[i].group);
            }
            if (tokens2[i].string_index != -1) {
                REQUIRE(tokens1[i].string_index == tokens2[i].string_index);
            }
        }
    }
}

TEST_CASE("GeX Hello", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {"He", "llo"});
    auto tokens2 = get_tokens("Hello", {0, 0, 1, 1, 1}, {0, 1, 0, 1, 2});

    test_tokens(tokens1, tokens2);
}

TEST_CASE("Basic macros", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {"A", "\\{\\%", "B\\relax C"});
    auto tokens2 = get_tokens("A{%BC", {0, 1, 1, 2, 2}, {0, 0, 2, 0, 8});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess(false, {"\\oopsy"}),
            "GeX compilation error in group 0 index 0: Undefined control "
            "sequence \"oopsy\"");
}

TEST_CASE("Defining basic macros", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {"a\\def\\aa{b}", "\\aa"});
    auto tokens2 = get_tokens("ab", {0, 1}, {0, 0});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess(false, {"\\def"}),
            "GeX compilation error in group 0 index 4: Expected control "
            "sequence");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def aa"}),
            "GeX compilation error in group 0 index 5: Expected control "
            "sequence");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa"}),
            "GeX compilation error in group 0 index 7: End of input reached "
            "while processing a definition");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa}"}),
            "GeX compilation error in group 0 index 7: Unexpected end group "
            "token");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa{"}),
            "GeX compilation error in group 0 index 8: End of input reached "
            "while processing a definition");
}

TEST_CASE("Macro expansion order", "[object][text][gex]") {
    auto tokens1 = preprocess(false,
        {"\\def\\aa{a\\def\\aa{b}}", "\\aa", "\\aa"});
    auto tokens2 = get_tokens("ab", {1, 2}, {0, 0});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa{\\def b}\\aa"}), "GeX "
        "compilation error in group 0 index 15: Error during macro expansion: "
        "Expected control sequence");
}

TEST_CASE("Macros with delimiters", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {"a  \\def\\aa |  |{b}", "\\aa| |"});
    auto tokens2 = get_tokens("a  b", {0, 0, 0, 1}, {0, 1, 2, 0});

    test_tokens(tokens1, tokens2);

    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa|{b}\\aa"}),
            "GeX compilation error in group 0 index 14: Input ended while "
            "processing macro \"aa\"");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa|{b}\\aa["}),
            "GeX compilation error in group 0 index 14: Use of \\aa does not "
            "match its definition");
}

TEST_CASE("Macros with parameters", "[object][text][gex]") {
    auto gex1_tokens = preprocess(false,
        {R"(\def\aa#1{a#1}\aa b)"});
    auto gex2_tokens = preprocess(false,
        {R"(\def\aa#1{a#1}\aa{b})"});
    auto gex3_tokens = preprocess(false,
        {R"(\def\aa#1{a#1}\aa{bb})"});
    auto gex4_tokens = preprocess(false,
        {R"(\def\aa#1#2{a#1b#2}\aa c d)"});
    auto gex5_tokens = preprocess(false,
        {R"(\def\aa#1#2{a#1b#2}\aa{cc}{dd})"});
    auto gex6_tokens = preprocess(false,
        {R"(\def\aa|#1 #2|{a#1b#2}\aa|cc dd|)"});
    auto gex7_tokens = preprocess(false,
        {R"(\def\aa|#1#2|{a#1b#2}\aa|cc dd|)"});
    auto gex8_tokens = preprocess(false,
        {R"(\def\aa|#1|{a#1b}\aa||)"});
    auto gex9_tokens = preprocess(false,
        {R"(\def\aa#1#2{a#1b#2c}\aa)", "{d}", "{e}"});
    auto gex10_tokens = preprocess(false,
        {R"(\def\aa#1,#2{a#1b#2}\aa{c,d},e)"});
    auto gex11_tokens = preprocess(false, {
        R"(\def\aa#1{a#1a})"
        R"(\def\bb#1{\aa#1})"
        R"(\bb b )"
        R"(\bb bb )"
        R"(\bb{bb} )"
        R"(\bb{{bb}})"
    });
    auto gex12_tokens = preprocess(false, {
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

    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa#2#1{b}"}),
            "GeX compilation error in group 0 index 7: Parameters must be "
            "numbered consecutively");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa#1{#2}"}),
            "GeX compilation error in group 0 index 10: Illegal parameter "
            "number");
    REQUIRE_THROWS_WITH(preprocess(false, {"\\def\\aa#1{#1}\\aa"}),
            "GeX compilation error in group 0 index 16: Input ended while "
            "processing macro \"aa\"");
}

TEST_CASE("preprocess style commands", "[object][text][gex]") {
    auto tokens = preprocess(false, {"\\displaystyle", "\\scriptstyle"});
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

TEST_CASE("GeX preprocess expandafter", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {
        R"(\def\aa#1{a#1a})"
        R"(\def\bb{bb})"
        R"(\aa\bb\ )" // Expands to a\bb a -> abba
        R"(\expandafter\aa\bb)" // Expands to \aa bb -> abab
    });
    auto tokens2 = preprocess(false, {
        R"(\def\aa#1{{a#1a}})"
        R"(\def\bb{bb})"
        // Expands to \expandafter\aa\aa bb -> \aa{aba}b -> {aabaa}b
        R"(\expandafter\expandafter\expandafter\aa\expandafter\aa\bb\ )"
        // Expands to \aa{a\bb a} -> \aa{abba} -> {aabbaa}
        R"(\expandafter\aa\aa\bb\ )"
        // Expands to {a\expandafter a}\aa\bb -> {aa}\aa\bb -> {aa}{a\bb a}
        // -> {aa}{abba}
        R"(\aa\expandafter\aa\bb\ )"
        // Expands to {a\aa a}\bb -> {a{aaa}}\bb -> {a{aaa}}bb
        R"(\aa\aa\bb)"
    });
    auto tokens3 = preprocess(false, {
        R"(\def\aa#1{{a#1a}})"
        R"(\def\bb{bb})"
        R"(\def\cc{\bb})"
        // Expands to \aa\bb -> {a\bb a} -> {abba}
        R"(\expandafter\aa\cc\ )"
        // Expands to \expandafter\aa\bb -> \aa bb -> {aba}b
        R"(\expandafter\expandafter\expandafter\aa\cc)"
    });
    REQUIRE_THROWS(preprocess(false, {
        R"(\def\aa#1{{a#1a}})"
        R"(\def\bb{bb})"
        R"(\expandafter{\aa}\bb)"
    }));
    // Ensure no arguments doesn't break it, having only one of the two
    // arguments was already tested above
    preprocess(false, {"\\expandafter"});
    auto ttokens1 = get_tokens("abba abab", 9, catcode_map2);
    auto ttokens2 = get_tokens(
            "{aabaa}b {aabbaa} {aa}{abba} {a{aaa}}bb", 39, catcode_map2);
    auto ttokens3 = get_tokens("{abba} {aba}b", 13, catcode_map2);
    test_tokens(tokens1, ttokens1);
    test_tokens(tokens2, ttokens2);
    test_tokens(tokens3, ttokens3);
}

TEST_CASE("GeX default global macros", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {"\\vec v"});
    auto tokens2 = preprocess(false, {"\\mathaccent →{v}"});
    REQUIRE(tokens1.size() == tokens2.size());
}

TEST_CASE("GeX macros add_base_macros", "[object][text][gex]") {
    // This initializes everything to make the next call not crash
    auto _ = MacroStack();
    MacroStack::add_base_macros(R"(\def\addbasemacrostest{a})");
    auto tokens = preprocess(false, {"\\addbasemacrostest"});
    REQUIRE(tokens.size() == 1);
    auto& character = get<CharacterToken>(tokens[0].value);
    REQUIRE(character.codepoint == U'a');
    REQUIRE(tokens[0].group == 0);
    REQUIRE(tokens[0].string_index == 0);
}

TEST_CASE("GeX \\def in groups", "[object][text][gex]") {
    auto tokens1 = preprocess(false, {
        R"(\def\aa{a})"
        R"(\aa)"
        R"({\def\aa{b}\aa})"
        R"(\aa)"
    });
    auto tokens2 = get_tokens("a{b}a", 5, catcode_map2);
    test_tokens(tokens1, tokens2);
}

TEST_CASE("GeX various fraction names", "[object][text][gex]") {
    auto tokens = preprocess(false, {
        R"(\over \atop \choose \above \overwithdelims()\atopwithdelims())"
    });
    REQUIRE(tokens.size() == 41);
    REQUIRE(get<CommandToken>(tokens[0].value).command_utf8
        == "abovewithdelims");
    REQUIRE(get<CharacterToken>(tokens[1].value).codepoint == U'.');
    REQUIRE(get<CharacterToken>(tokens[2].value).codepoint == U'.');
    REQUIRE(get<CharacterToken>(tokens[3].value).codepoint == U' ');
    REQUIRE(get<CharacterToken>(tokens[4].value).codepoint == U'-');
    REQUIRE(get<CharacterToken>(tokens[5].value).codepoint == U'1');
    REQUIRE(get<CharacterToken>(tokens[6].value).codepoint == U'p');
    REQUIRE(get<CharacterToken>(tokens[7].value).codepoint == U't');
    REQUIRE(get<CommandToken>(tokens[8].value).command_utf8
        == "abovewithdelims");
    REQUIRE(get<CharacterToken>(tokens[9].value).codepoint == U'.');
    REQUIRE(get<CharacterToken>(tokens[10].value).codepoint == U'.');
    REQUIRE(get<CharacterToken>(tokens[11].value).codepoint == U' ');
    REQUIRE(get<CharacterToken>(tokens[12].value).codepoint == U'0');
    REQUIRE(get<CharacterToken>(tokens[13].value).codepoint == U'p');
    REQUIRE(get<CharacterToken>(tokens[14].value).codepoint == U't');
    REQUIRE(get<CommandToken>(tokens[15].value).command_utf8
        == "abovewithdelims");
    REQUIRE(get<CharacterToken>(tokens[16].value).codepoint == U'(');
    REQUIRE(get<CharacterToken>(tokens[17].value).codepoint == U')');
    REQUIRE(get<CharacterToken>(tokens[18].value).codepoint == U' ');
    REQUIRE(get<CharacterToken>(tokens[19].value).codepoint == U'0');
    REQUIRE(get<CharacterToken>(tokens[20].value).codepoint == U'p');
    REQUIRE(get<CharacterToken>(tokens[21].value).codepoint == U't');
    REQUIRE(get<CommandToken>(tokens[22].value).command_utf8
        == "abovewithdelims");
    REQUIRE(get<CharacterToken>(tokens[23].value).codepoint == U'.');
    REQUIRE(get<CharacterToken>(tokens[24].value).codepoint == U'.');
    REQUIRE(get<CharacterToken>(tokens[25].value).codepoint == U' ');
    REQUIRE(get<CommandToken>(tokens[26].value).command_utf8
        == "abovewithdelims");
    REQUIRE(get<CharacterToken>(tokens[27].value).codepoint == U'(');
    REQUIRE(get<CharacterToken>(tokens[28].value).codepoint == U')');
    REQUIRE(get<CharacterToken>(tokens[29].value).codepoint == U' ');
    REQUIRE(get<CharacterToken>(tokens[30].value).codepoint == U'-');
    REQUIRE(get<CharacterToken>(tokens[31].value).codepoint == U'1');
    REQUIRE(get<CharacterToken>(tokens[32].value).codepoint == U'p');
    REQUIRE(get<CharacterToken>(tokens[33].value).codepoint == U't');
    REQUIRE(get<CommandToken>(tokens[34].value).command_utf8
        == "abovewithdelims");
    REQUIRE(get<CharacterToken>(tokens[35].value).codepoint == U'(');
    REQUIRE(get<CharacterToken>(tokens[36].value).codepoint == U')');
    REQUIRE(get<CharacterToken>(tokens[37].value).codepoint == U' ');
    REQUIRE(get<CharacterToken>(tokens[38].value).codepoint == U'0');
    REQUIRE(get<CharacterToken>(tokens[39].value).codepoint == U'p');
    REQUIRE(get<CharacterToken>(tokens[40].value).codepoint == U't');
}

TEST_CASE("GeX preprocessor sqrt fraction", "[object][text][gex]") {
    auto tokens = preprocess(false, {R"(\sqrt{{a \over b}})"});
    REQUIRE(tokens.size() == 16);
}
