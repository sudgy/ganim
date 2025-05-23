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
