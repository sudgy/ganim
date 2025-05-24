#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "ganim/object/text/gex/gex.hpp"
#include "ganim/object/text/text_helpers.hpp"

using namespace ganim;

namespace {
    void test_glyph(
        PositionedGlyph glyph1,
        PositionedGlyph glyph2,
        int index = -1
    )
    {
        INFO("index " << index);
        REQUIRE(glyph1.x_pos == glyph2.x_pos);
        REQUIRE(glyph1.y_pos == glyph2.y_pos);
        REQUIRE(glyph1.draw_x == glyph2.draw_x);
        REQUIRE(glyph1.draw_y == glyph2.draw_y);
        REQUIRE(glyph1.width == glyph2.width);
        REQUIRE(glyph1.height == glyph2.height);
        REQUIRE(glyph1.y_min == glyph2.y_min);
        REQUIRE(glyph1.y_max == glyph2.y_max);
        REQUIRE(glyph1.texture_x == glyph2.texture_x);
        REQUIRE(glyph1.texture_y == glyph2.texture_y);
        REQUIRE(glyph1.texture_width == glyph2.texture_width);
        REQUIRE(glyph1.texture_height == glyph2.texture_height);
        REQUIRE(glyph1.group_index == glyph2.group_index);
    }
}

TEST_CASE("GeX Hello", "[object][text][gex]") {
    auto gex = GeX({"He", "llo"});
    auto glyphs1 = gex.get_output();

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs2 = shape_text(font, {U"He", U"llo"});

    REQUIRE(glyphs1.size() == glyphs2.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(glyphs1[i], glyphs2[i], i);
    }
}

TEST_CASE("Basic macros", "[object][text][gex]") {
    auto gex = GeX({"A", "\\{\\%", "B\\relax C"});
    auto glyphs1 = gex.get_output();

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs2 = shape_text(font, {U"A", U"{%", U"BC"});

    REQUIRE(glyphs1.size() == glyphs2.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(glyphs1[i], glyphs2[i], i);
    }

    auto bad = GeX({"\\oopsy"});
    REQUIRE_THROWS_WITH(bad.get_output(), "GeX compilation error in group 0 "
            "index 0: Undefined control sequence \"oopsy\"");
}

TEST_CASE("Defining basic macros", "[object][text][gex]") {
    auto gex = GeX({"a\\def\\aa{b}", "\\aa"});
    auto glyphs1 = gex.get_output();

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs2 = shape_text(font, {U"a", U"b"});

    REQUIRE(glyphs1.size() == glyphs2.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(glyphs1[i], glyphs2[i], i);
    }

    auto bad1 = GeX({"\\def"});
    REQUIRE_THROWS_WITH(bad1.get_output(), "GeX compilation error in group 0 "
            "index 4: Expected control sequence");
    auto bad2 = GeX({"\\def aa"});
    REQUIRE_THROWS_WITH(bad2.get_output(), "GeX compilation error in group 0 "
            "index 5: Expected control sequence");
    auto bad3 = GeX({"\\def\\aa"});
    REQUIRE_THROWS_WITH(bad3.get_output(), "GeX compilation error in group 0 "
            "index 7: End of input reached while processing a definition");
    auto bad4 = GeX({"\\def\\aa}"});
    REQUIRE_THROWS_WITH(bad4.get_output(), "GeX compilation error in group 0 "
            "index 7: Unexpected end group token");
    auto bad5 = GeX({"\\def\\aa{"});
    REQUIRE_THROWS_WITH(bad5.get_output(), "GeX compilation error in group 0 "
            "index 8: End of input reached while processing a definition");
}

TEST_CASE("Macro expansion order", "[object][text][gex]") {
    auto gex = GeX({"\\def\\aa{a\\def\\aa{b}}", "\\aa", "\\aa"});
    auto glyphs1 = gex.get_output();

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs2 = shape_text(font, {U"", U"a", U"b"});

    REQUIRE(glyphs1.size() == glyphs2.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(glyphs1[i], glyphs2[i], i);
    }

    auto bad = GeX({"\\def\\aa{\\def b}\\aa"});
    REQUIRE_THROWS_WITH(bad.get_output(), "GeX compilation error in group 0 "
        "index 15: Error during macro expansion: Expected control sequence");
}

TEST_CASE("Macros with delimiters", "[object][text][gex]") {
    auto gex = GeX({"a  \\def\\aa |  |{b}", "\\aa| |"});
    auto glyphs1 = gex.get_output();

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs2 = shape_text(font, {U"a  ", U"b"});

    REQUIRE(glyphs1.size() == glyphs2.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(glyphs1[i], glyphs2[i], i);
    }

    auto bad1 = GeX({"\\def\\aa|{b}\\aa"});
    REQUIRE_THROWS_WITH(bad1.get_output(), "GeX compilation error in group 0 "
            "index 14: Input ended while processing macro \"aa\"");
    auto bad2 = GeX({"\\def\\aa|{b}\\aa["});
    REQUIRE_THROWS_WITH(bad2.get_output(), "GeX compilation error in group 0 "
            "index 14: Use of \\aa does not match its definition");
}

TEST_CASE("Macros with parameters", "[object][text][gex]") {
    auto gex1 = GeX({R"(\def\aa#1{a#1}\aa b)"});
    auto gex2 = GeX({R"(\def\aa#1{a#1}\aa{b})"});
    auto gex3 = GeX({R"(\def\aa#1{a#1}\aa{bb})"});
    auto gex4 = GeX({R"(\def\aa#1#2{a#1b#2}\aa c d)"});
    auto gex5 = GeX({R"(\def\aa#1#2{a#1b#2}\aa{cc}{dd})"});
    auto gex6 = GeX({R"(\def\aa|#1 #2|{a#1b#2}\aa|cc dd|)"});
    auto gex7 = GeX({R"(\def\aa|#1#2|{a#1b#2}\aa|cc dd|)"});
    auto gex8 = GeX({R"(\def\aa|#1|{a#1b}\aa||)"});
    auto gex9 = GeX({R"(\def\aa#1#2{a#1b#2c}\aa)", "{d}", "{e}"});
    auto gex10 = GeX({R"(\def\aa#1,#2{a#1b#2}\aa{c,d},e)"});
    auto gex11 = GeX({
        R"(\def\aa#1{a#1a})"
        R"(\def\bb#1{\aa#1})"
        R"(\bb b )"
        R"(\bb bb )"
        R"(\bb{bb} )"
        R"(\bb{{bb}})"
    });
    auto gex12 = GeX({
        R"(\def\aa#1{a#1a})"
        R"(\def\bb#1|{\aa#1})"
        R"(\bb bb| )"
        R"(\bb{bb}| )"
        R"(\bb{bb}c|)"
    });
    auto gex1_glyphs = gex1.get_output();
    auto gex2_glyphs = gex2.get_output();
    auto gex3_glyphs = gex3.get_output();
    auto gex4_glyphs = gex4.get_output();
    auto gex5_glyphs = gex5.get_output();
    auto gex6_glyphs = gex6.get_output();
    auto gex7_glyphs = gex7.get_output();
    auto gex8_glyphs = gex8.get_output();
    auto gex9_glyphs = gex9.get_output();
    auto gex10_glyphs = gex10.get_output();
    auto gex11_glyphs = gex11.get_output();
    auto gex12_glyphs = gex12.get_output();

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs1 = shape_text(font, {U"ab"});
    auto glyphs3 = shape_text(font, {U"abb"});
    auto glyphs4 = shape_text(font, {U"acbd"});
    auto glyphs5 = shape_text(font, {U"accbdd"});
    auto glyphs7 = shape_text(font, {U"acbc dd"});
    auto glyphs8 = shape_text(font, {U"ab"});
    auto glyphs9 = shape_text_manual_groups(font,
            {{U"a", 0}, {U"d", 1}, {U"b", 0}, {U"e", 2}, {U"c", 0}});
    auto glyphs10 = shape_text(font, {U"ac,dbe"});
    auto glyphs11 = shape_text(font, {U"aba abab abab abba"});
    auto glyphs12 = shape_text(font, {U"abab abab abbac"});

    REQUIRE(gex1_glyphs.size() == glyphs1.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(gex1_glyphs[i], glyphs1[i], i);
    }
    REQUIRE(gex2_glyphs.size() == glyphs1.size());
    for (int i = 0; i < ssize(glyphs1); ++i) {
        test_glyph(gex2_glyphs[i], glyphs1[i], i);
    }
    REQUIRE(gex3_glyphs.size() == glyphs3.size());
    for (int i = 0; i < ssize(glyphs3); ++i) {
        test_glyph(gex3_glyphs[i], glyphs3[i], i);
    }
    REQUIRE(gex4_glyphs.size() == glyphs4.size());
    for (int i = 0; i < ssize(glyphs4); ++i) {
        test_glyph(gex4_glyphs[i], glyphs4[i], i);
    }
    REQUIRE(gex5_glyphs.size() == glyphs5.size());
    for (int i = 0; i < ssize(glyphs5); ++i) {
        test_glyph(gex5_glyphs[i], glyphs5[i], i);
    }
    REQUIRE(gex6_glyphs.size() == glyphs5.size());
    for (int i = 0; i < ssize(glyphs5); ++i) {
        test_glyph(gex6_glyphs[i], glyphs5[i], i);
    }
    REQUIRE(gex7_glyphs.size() == glyphs7.size());
    for (int i = 0; i < ssize(glyphs7); ++i) {
        test_glyph(gex7_glyphs[i], glyphs7[i], i);
    }
    REQUIRE(gex8_glyphs.size() == glyphs8.size());
    for (int i = 0; i < ssize(glyphs8); ++i) {
        test_glyph(gex8_glyphs[i], glyphs8[i], i);
    }
    REQUIRE(gex9_glyphs.size() == glyphs9.size());
    for (int i = 0; i < ssize(glyphs9); ++i) {
        test_glyph(gex9_glyphs[i], glyphs9[i], i);
    }
    REQUIRE(gex10_glyphs.size() == glyphs10.size());
    for (int i = 0; i < ssize(glyphs10); ++i) {
        test_glyph(gex10_glyphs[i], glyphs10[i], i);
    }
    REQUIRE(gex11_glyphs.size() == glyphs11.size());
    for (int i = 0; i < ssize(glyphs11); ++i) {
        test_glyph(gex11_glyphs[i], glyphs11[i], i);
    }
    REQUIRE(gex12_glyphs.size() == glyphs12.size());
    for (int i = 0; i < ssize(glyphs12); ++i) {
        test_glyph(gex12_glyphs[i], glyphs12[i], i);
    }

    auto bad1 = GeX({"\\def\\aa#2#1{b}"});
    REQUIRE_THROWS_WITH(bad1.get_output(), "GeX compilation error in group 0 "
            "index 7: Parameters must be numbered consecutively");
    auto bad2 = GeX({"\\def\\aa#1{#2}"});
    REQUIRE_THROWS_WITH(bad2.get_output(), "GeX compilation error in group 0 "
            "index 10: Illegal parameter number");
    auto bad3 = GeX({"\\def\\aa#1{#1}\\aa"});
    REQUIRE_THROWS_WITH(bad3.get_output(), "GeX compilation error in group 0 "
            "index 16: Input ended while processing macro \"aa\"");
}
