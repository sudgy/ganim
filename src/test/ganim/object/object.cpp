#include <catch2/catch_test_macros.hpp>

#include "ganim/object/object.hpp"

using namespace ganim;

TEST_CASE("Object color", "[object]") {
    auto test = Object();
    REQUIRE(test.get_color() == Color("FFFFFF"));
    test.set_color(Color("AAAAAAAA"));
    REQUIRE(test.get_color() == Color("AAAAAAFF"));
    test.set_color_with_alpha(Color("BBBBBBBB"));
    REQUIRE(test.get_color() == Color("BBBBBBBB"));
    test.set_opacity(0.5);
    REQUIRE(test.get_color() == Color("BBBBBB7F"));
}
