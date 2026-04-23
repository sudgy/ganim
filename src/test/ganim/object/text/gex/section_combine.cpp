#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ganim/object/text/gex/section_combine.hpp"

using namespace ganim;
using namespace ganim::gex;
using namespace Catch::Matchers;

TEST_CASE("section_combine", "[object][text][gex]") {
    auto glyph = std::vector<Glyph>{{.x_pos = 0, .y_pos = 0}};
    auto glyphs1 = section_combine({}).glyphs;
    auto glyphs2 = section_combine({Box(1, 2, 3, glyph)}).glyphs;
    auto glyphs3 = section_combine(
        {Box(1, 2, 3, glyph), Box(2, 3, 2, glyph)}).glyphs;

    REQUIRE(glyphs1.size() == 0);

    REQUIRE(glyphs2.size() == 1);
    REQUIRE(glyphs2[0].x_pos == 0);
    REQUIRE(glyphs2[0].y_pos == 0);

    REQUIRE(glyphs3.size() == 2);
    REQUIRE(glyphs3[0].x_pos == 0);
    REQUIRE(glyphs3[0].y_pos == 0);
    REQUIRE(glyphs3[1].x_pos == 1);
    REQUIRE(glyphs3[1].y_pos == 0);
}

TEST_CASE("section_combine spacing", "[object][text][gex]") {
    auto box1 = Box(1, 1, 1, {});
    auto box2 = Box(1, 1, 1, {});
    auto box3 = Box(1, 1, 1, {});
    auto box4 = Box(1, 1, 1, {});
    auto box5 = Box(1, 1, 1, {});
    box1.glyphs.emplace_back();
    box2.glyphs.emplace_back();
    box3.glyphs.emplace_back();
    box4.glyphs.emplace_back();
    box5.glyphs.emplace_back();
    box1.glyphs[0].width = 1;
    box2.glyphs[0].width = 0.5;
    box3.glyphs[0].width = 1;
    box4.glyphs[0].x_pos = 0.5;
    box4.glyphs[0].width = 0.5;

    auto glyphs = section_combine({box1, box2, box3, box4, box5}).glyphs;
    REQUIRE(glyphs.size() == 5);
    REQUIRE(glyphs[0].x_pos == 0);
    REQUIRE_THAT(glyphs[1].x_pos, WithinAbs(1.1, 1e-5));
    REQUIRE_THAT(glyphs[2].x_pos, WithinAbs(2.1, 1e-5));
    REQUIRE_THAT(glyphs[3].x_pos, WithinAbs(3.6, 1e-5));
    REQUIRE_THAT(glyphs[4].x_pos, WithinAbs(4.2, 1e-5));
}
