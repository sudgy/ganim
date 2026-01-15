#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/box.hpp"

using namespace ganim;
using namespace ganim::gex;

TEST_CASE("combine_boxes_horizontally", "[object][text][gex]") {
    auto glyph = std::vector<Glyph>{{.x_pos = 0, .y_pos = 0}};
    auto test1 = combine_boxes_horizontally({});
    auto test2 = combine_boxes_horizontally({Box(1, 2, 3, glyph)});
    auto test3 = combine_boxes_horizontally(
        {Box(1, 2, 3, glyph), Box(1, 2, 3, glyph)});
    auto test4 = combine_boxes_horizontally(
        {Box(1, 2, 3, glyph), Box(2, 3, 2, glyph)});

    REQUIRE(test1.width == 0);
    REQUIRE(test1.height == 0);
    REQUIRE(test1.depth == 0);
    REQUIRE(test1.glyphs.size() == 0);

    REQUIRE(test2.width == 1);
    REQUIRE(test2.height == 2);
    REQUIRE(test2.depth == 3);
    REQUIRE(test2.glyphs.size() == 1);
    REQUIRE(test2.glyphs[0].x_pos == 0.0);
    REQUIRE(test2.glyphs[0].y_pos == 0.0);

    REQUIRE(test3.width == 2);
    REQUIRE(test3.height == 2);
    REQUIRE(test3.depth == 3);
    REQUIRE(test3.glyphs.size() == 2);
    REQUIRE(test3.glyphs[0].x_pos == 0.0);
    REQUIRE(test3.glyphs[0].y_pos == 0.0);
    REQUIRE(test3.glyphs[1].x_pos == 1.0);
    REQUIRE(test3.glyphs[1].y_pos == 0.0);

    REQUIRE(test4.width == 3);
    REQUIRE(test4.height == 3);
    REQUIRE(test4.depth == 3);
    REQUIRE(test3.glyphs.size() == 2);
    REQUIRE(test3.glyphs[0].x_pos == 0.0);
    REQUIRE(test3.glyphs[0].y_pos == 0.0);
    REQUIRE(test3.glyphs[1].x_pos == 1.0);
    REQUIRE(test3.glyphs[1].y_pos == 0.0);
}

TEST_CASE("combine_boxes_vertically", "[object][text][gex]") {
    auto glyph = std::vector<Glyph>{{.x_pos = 0, .y_pos = 0}};
    auto test1 = combine_boxes_vertically({}, 0);
    auto test2 = combine_boxes_vertically({Box(1, 2, 3, glyph)}, 0);
    auto test3 = combine_boxes_vertically(
            {Box(1, 2, 3, glyph), Box(1, 2, 3, glyph)}, 0);
    auto test4 = combine_boxes_vertically(
            {Box(1, 2, 3, glyph), Box(1, 2, 3, glyph)}, 1);
    auto test5 = combine_boxes_vertically(
            {Box(1, 2, 3, glyph), Box(2, 3, 2, glyph)}, 0);
    auto test6 = combine_boxes_vertically(
            {Box(1, 2, 3, glyph), Box(2, 3, 2, glyph)}, 1);

    REQUIRE(test1.width == 0);
    REQUIRE(test1.height == 0);
    REQUIRE(test1.depth == 0);
    REQUIRE(test1.glyphs.size() == 0);

    REQUIRE(test2.width == 1);
    REQUIRE(test2.height == 2);
    REQUIRE(test2.depth == 3);
    REQUIRE(test2.glyphs.size() == 1);
    REQUIRE(test2.glyphs[0].x_pos == 0.0);
    REQUIRE(test2.glyphs[0].y_pos == 0.0);

    REQUIRE(test3.width == 1);
    REQUIRE(test3.height == 2);
    REQUIRE(test3.depth == 8);
    REQUIRE(test3.glyphs.size() == 2);
    REQUIRE(test3.glyphs[0].x_pos == 0.0);
    REQUIRE(test3.glyphs[0].y_pos == 0.0);
    REQUIRE(test3.glyphs[1].x_pos == 0.0);
    REQUIRE(test3.glyphs[1].y_pos == -5.0);

    REQUIRE(test4.width == 1);
    REQUIRE(test4.height == 7);
    REQUIRE(test4.depth == 3);
    REQUIRE(test4.glyphs.size() == 2);
    REQUIRE(test4.glyphs[0].x_pos == 0.0);
    REQUIRE(test4.glyphs[0].y_pos == 5.0);
    REQUIRE(test4.glyphs[1].x_pos == 0.0);
    REQUIRE(test4.glyphs[1].y_pos == 0.0);

    REQUIRE(test5.width == 2);
    REQUIRE(test5.height == 2);
    REQUIRE(test5.depth == 8);
    REQUIRE(test5.glyphs.size() == 2);
    REQUIRE(test5.glyphs[0].x_pos == 0.0);
    REQUIRE(test5.glyphs[0].y_pos == 0.0);
    REQUIRE(test5.glyphs[1].x_pos == 0.0);
    REQUIRE(test5.glyphs[1].y_pos == -6.0);

    REQUIRE(test6.width == 2);
    REQUIRE(test6.height == 8);
    REQUIRE(test6.depth == 2);
    REQUIRE(test6.glyphs.size() == 2);
    REQUIRE(test6.glyphs[0].x_pos == 0.0);
    REQUIRE(test6.glyphs[0].y_pos == 6.0);
    REQUIRE(test6.glyphs[1].x_pos == 0.0);
    REQUIRE(test6.glyphs[1].y_pos == 0.0);
}

TEST_CASE("combine_boxes_vertically overlapping objects?",
        "[object][text][gex]")
{
    auto glyph1 = Glyph();
    auto glyph2 = Glyph();
    auto glyph3 = Glyph();
    glyph1.draw_y = 0.5;
    glyph2.draw_y = 0.1;
    glyph3.draw_y = 0.7;
    auto test = combine_boxes_vertically({
        Box(1, 0.5, 0.01, {glyph1}),
        Box(1, 0.1, 0, {glyph2}),
        Box(1, 0.7, 0.01, {glyph3}),
    }, 0);
    REQUIRE(test.glyphs[1].draw_y > test.glyphs[2].draw_y);
}
