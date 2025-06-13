#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/section_combine.hpp"

using namespace ganim;
using namespace ganim::gex;

TEST_CASE("section_combine", "[object][text][gex]") {
    auto glyph = std::vector<PositionedGlyph>{{.x_pos = 0, .y_pos = 0}};
    auto glyphs1 = section_combine({});
    auto glyphs2 = section_combine({Box(1, 2, 3, glyph)});
    auto glyphs3 = section_combine({Box(1, 2, 3, glyph), Box(2, 3, 2, glyph)});

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
