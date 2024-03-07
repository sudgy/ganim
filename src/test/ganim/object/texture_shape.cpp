#include <catch2/catch_test_macros.hpp>

#include "ganim/object/texture_shape.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

TEST_CASE("TextureShape drawing", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 1);
    auto image1 = std::array<unsigned char, 16>{
        255, 0, 0, 255,   0, 255, 0, 255,
        0, 0, 255, 255,   255, 255, 255, 255
    };
    auto image2 = std::array<unsigned char, 16>{
        255, 255, 0, 255,   0, 255, 255, 255,
        255, 0, 255, 255,   255, 255, 255, 255
    };
    auto texture1 = gl::Texture(image1.data(), 2, 2);
    auto texture2 = gl::Texture(image2.data(), 2, 2);
    auto shape = TextureShape<Shape>(
        {{ 1,  1, 0},
         { 1, -1, 0},
         {-1, -1, 0},
         {-1,  1, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    shape.set_texture_vertices(
        {{ 0,  0},
         { 0, -1},
         {-1, -1},
         {-1,  0}}
    );
    shape.set_texture(texture1);
    shape.set_visible(true);
    scene.add(shape);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 1, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 1, 1) == Color("0000FF"));
    REQUIRE(scene.get_pixel(0, 2, 1) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 1, 2) == Color("FF0000"));
    REQUIRE(scene.get_pixel(0, 2, 2) == Color("00FF00"));
    shape.set_texture(texture2);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(1, 1, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 1, 1) == Color("FF00FF"));
    REQUIRE(scene.get_pixel(1, 2, 1) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(1, 1, 2) == Color("FFFF00"));
    REQUIRE(scene.get_pixel(1, 2, 2) == Color("00FFFF"));
}
