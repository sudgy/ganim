#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/section_render.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    TokenList convert_to_tokens(std::string_view str)
    {
        auto result = TokenList();
        int i = 0;
        for (auto c : str) {
            auto cat = CategoryCode::Letter;
            if (c == '$') cat = CategoryCode::MathShift;
            if (c == '{') cat = CategoryCode::StartGroup;
            if (c == '}') cat = CategoryCode::EndGroup;
            result.emplace_back(CharacterToken(c, cat), 0, i++);
        }
        return result;
    }
    void check_glyphs(
        const std::vector<PositionedGlyph>& glyphs1,
        const std::vector<PositionedGlyph>& glyphs2
    )
    {
        REQUIRE(glyphs1.size() == glyphs2.size());
        for (int i = 0; i < ssize(glyphs1); ++i) {
            REQUIRE(glyphs1[i].x_pos == glyphs2[i].x_pos);
            REQUIRE(glyphs1[i].y_pos == glyphs2[i].y_pos);
            REQUIRE(glyphs1[i].draw_x == glyphs2[i].draw_x);
            REQUIRE(glyphs1[i].draw_y == glyphs2[i].draw_y);
            REQUIRE(glyphs1[i].width == glyphs2[i].width);
            REQUIRE(glyphs1[i].height == glyphs2[i].height);
            REQUIRE(glyphs1[i].y_min == glyphs2[i].y_min);
            REQUIRE(glyphs1[i].y_max == glyphs2[i].y_max);
            REQUIRE(glyphs1[i].texture_x == glyphs2[i].texture_x);
            REQUIRE(glyphs1[i].texture_y == glyphs2[i].texture_y);
            REQUIRE(glyphs1[i].texture_width == glyphs2[i].texture_width);
            REQUIRE(glyphs1[i].texture_height == glyphs2[i].texture_height);
            REQUIRE(glyphs1[i].group_index == glyphs2[i].group_index);
        }
    }
}

TEST_CASE("GeX text section render", "[object][text][gex]") {
    auto section1 = section_render(
        Section(convert_to_tokens("a"), Section::Text)
    );
    auto section2 = section_render(
        Section(convert_to_tokens("ag"), Section::Text)
    );

    REQUIRE(section1.width == 0.46875);
    REQUIRE(section1.height == 0.453125);
    REQUIRE(section1.depth == 0.015625);
    REQUIRE(section2.width == 0.984375);
    REQUIRE(section2.height == 0.4609375);
    REQUIRE(section2.depth == 0.2109375);

    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto glyphs1 = shape_text(font, {U"a"});
    auto glyphs2 = shape_text(font, {U"ag"});

    check_glyphs(section1.glyphs, glyphs1);
    check_glyphs(section2.glyphs, glyphs2);
}

TEST_CASE("GeX section_render braces in text mode", "[object][text][gex]") {
    auto section = section_render(
            Section(convert_to_tokens("a{bc}"), Section::Text));
    REQUIRE(section.glyphs.size() == 3);
}
