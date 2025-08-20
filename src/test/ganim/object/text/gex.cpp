#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex.hpp"

using namespace ganim;

TEST_CASE("make_gex", "[object][text]") {
    auto test1 = make_gex("a  b");
    auto test2 = make_gext("a  b");
    REQUIRE(test1->size() == 2);
    REQUIRE(test2->size() == 2);

    auto shape1 = dynamic_cast<TextureShape<Shape>&>(*test1[0]);
    auto shape2 = dynamic_cast<TextureShape<Shape>&>(*test2[0]);
    auto& vertices1 = shape1.get_texture_vertices();
    auto& vertices2 = shape2.get_texture_vertices();
    REQUIRE((vertices1[0].x != vertices2[0].x or
            vertices1[0].y != vertices2[0].y));
}
