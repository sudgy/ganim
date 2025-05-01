#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/text_helpers.hpp"

using namespace ganim;

TEST_CASE("Text helpers", "[object][text]") {
    auto& font = get_font("fonts/NewCM10-Regular.otf");
    REQUIRE(get_font_ascender(font) == 0.75);
    REQUIRE(get_font_descender(font) == -0.25);

    auto shaped = shape_text(font, {U"Wo", U"rld"});
    REQUIRE(shaped.size() == 5);
    // W
    REQUIRE(shaped[0].x_pos == 0);
    REQUIRE(shaped[0].y_pos == 0);
    REQUIRE(shaped[0].group_index == 0);
    REQUIRE(shaped[0].glyph->texture_width == 0.126953125);
    REQUIRE(shaped[0].glyph->texture_height == 0.08984375);
    REQUIRE(shaped[0].glyph->width == 1.015625);
    REQUIRE(shaped[0].glyph->height == 0.71875);
    REQUIRE(shaped[0].glyph->bearing_x == 0.0078125);
    REQUIRE(shaped[0].glyph->bearing_y == 0.6875);
    // o
    REQUIRE(shaped[1].x_pos == 0.9449462890625);
    REQUIRE(shaped[1].y_pos == 0);
    REQUIRE(shaped[1].group_index == 0);
    REQUIRE(shaped[1].glyph->texture_width == 0.05859375);
    REQUIRE(shaped[1].glyph->texture_height == 0.05859375);
    REQUIRE(shaped[1].glyph->width == 0.46875);
    REQUIRE(shaped[1].glyph->height == 0.46875);
    REQUIRE(shaped[1].glyph->bearing_x == 0.015625);
    REQUIRE(shaped[1].glyph->bearing_y == 0.453125);
    // r
    REQUIRE(shaped[2].x_pos == 1.4449462890625);
    REQUIRE(shaped[2].y_pos == 0);
    REQUIRE(shaped[2].group_index == 1);
    REQUIRE(shaped[2].glyph->texture_width == 0.044921875);
    REQUIRE(shaped[2].glyph->texture_height == 0.057617188f);
    REQUIRE(shaped[2].glyph->width == 0.359375);
    REQUIRE(shaped[2].glyph->height == 0.4609375);
    REQUIRE(shaped[2].glyph->bearing_x == 0.015625);
    REQUIRE(shaped[2].glyph->bearing_y == 0.453125);
    // l
    REQUIRE(shaped[3].x_pos == 1.8369140625);
    REQUIRE(shaped[3].y_pos == 0);
    REQUIRE(shaped[3].group_index == 1);
    REQUIRE(shaped[3].glyph->texture_width == 0.030273438f);
    REQUIRE(shaped[3].glyph->texture_height == 0.088867188f);
    REQUIRE(shaped[3].glyph->width == 0.2421875);
    REQUIRE(shaped[3].glyph->height == 0.7109375);
    REQUIRE(shaped[3].glyph->bearing_x == 0.0234375);
    REQUIRE(shaped[3].glyph->bearing_y == 0.703125);
    // d
    REQUIRE(shaped[4].x_pos == 2.1148681640625);
    REQUIRE(shaped[4].y_pos == 0);
    REQUIRE(shaped[4].group_index == 1);
    REQUIRE(shaped[4].glyph->texture_width == 0.064453125);
    REQUIRE(shaped[4].glyph->texture_height == 0.08984375);
    REQUIRE(shaped[4].glyph->width == 0.515625);
    REQUIRE(shaped[4].glyph->height == 0.71875);
    REQUIRE(shaped[4].glyph->bearing_x == 0.0234375);
    REQUIRE(shaped[4].glyph->bearing_y == 0.703125);
}

// There is a test for set_text_texture_size but it's in text.cpp because it's
// easier to test there
