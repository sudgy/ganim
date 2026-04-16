#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/gex.hpp"

using namespace ganim;

TEST_CASE("GeX spaces between sections", "[object][text][gex]") {
    auto result1 = gex_render(false, {"a$n$"});
    auto result2 = gex_render(false, {"a $n$"});
    REQUIRE(result2.glyphs.back().x_pos > result1.glyphs.back().x_pos + 0.1);
}

TEST_CASE("GeX minus sign spacing", "[object][text][gex]") {
    auto result1 = gex_render(false, {"$4-1$"});
    auto result2 = gex_render(false, {"$4{-}1$"});
    REQUIRE(result1.glyphs.back().x_pos > result2.glyphs.back().x_pos + 0.1);
}

TEST_CASE("GeX \\text styles", "[object][text][gex]") {
    auto result1 = gex_render(false, {"$$\\text{$a \\over b$}$$"});
    auto result2 = gex_render(false, {"$$a \\over b$$"});
    REQUIRE(result1.glyphs[0].x_pos == result2.glyphs[0].x_pos);
}
