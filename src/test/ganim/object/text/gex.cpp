#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/text/gex.hpp"

using namespace ganim;

TEST_CASE("make_gex", "[object][text]") {
    auto test1 = make_gex("a  b");
    auto test2 = make_gext("a  b");
    REQUIRE(test1->size() == 2);
    REQUIRE(test2->size() == 2);

    auto shape1 = dynamic_cast<TextureShape<Shape>&>(*test1[0][0]);
    auto shape2 = dynamic_cast<TextureShape<Shape>&>(*test2[0][0]);
    auto& vertices1 = shape1.get_texture_vertices();
    auto& vertices2 = shape2.get_texture_vertices();
    REQUIRE((vertices1[0].x != vertices2[0].x or
            vertices1[0].y != vertices2[0].y));
}

TEST_CASE("GeX \\phantom", "[object][text][gex]") {
    auto result1 = make_gex("{a}  b");
    auto result2 = make_gex("\\phantom{a}  b");
    auto result3 = make_gex("c  \\phantom{a} b");
    auto result4 = make_gex("c  \\phantom{a}  b");
    REQUIRE(result1->get_width() == result2->get_width());
    REQUIRE(
        result4[2]->get_center<VGA2>().blade_project<vga2::e1>() >
        result3[1]->get_center<VGA2>().blade_project<vga2::e1>()
    );
    REQUIRE(result3[1]->size() == 2);
    REQUIRE(result4->size() == 3);

    auto result5 = make_gex("\\phantom{a}");
    auto scene = TestScene(16, 16, 4, 4, 1);
    result5->set_visible(true);
    scene.add(result5);
    scene.frame_advance();
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            REQUIRE(scene.get_pixel(0, x, y) == Color("000000"));
        }
    }
}
