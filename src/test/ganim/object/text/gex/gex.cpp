#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/gex.hpp"

using namespace ganim;

TEST_CASE("GeX spaces between sections", "[object][text][gex]") {
    auto result1 = gex_render({"a$n$"});
    auto result2 = gex_render({"a $n$"});
    REQUIRE(result2.back().x_pos > result1.back().x_pos + 0.1);
}
