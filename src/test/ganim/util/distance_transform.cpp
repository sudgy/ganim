#include <catch2/catch_test_macros.hpp>

#include "ganim/util/distance_transform.hpp"

#include <cmath>

#include "ganim/gl/gl.hpp"

using namespace ganim;

TEST_CASE("distance_transform", "[object]") {
    const auto image = std::array<unsigned char, 8*8>{
        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    const auto sq2 = float(std::sqrt(2));
    const auto sq5 = float(std::sqrt(5));
    const auto sq8 = float(std::sqrt(8));
    const auto sqa = float(std::sqrt(10));
    const auto sqd = float(std::sqrt(13));
    const auto transform = std::array<float, 8*8>{
        0.0, 1.0, 2.0, 2.0, 2.0, 2.0, sq5, sq8,
        1.0, sq2, 1.0, 1.0, 1.0, 1.0, sq2, sq5,
        2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0,
        2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 2.0,
        2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 2.0,
        2.0, sq2, 1.0, sq2, sq2, 1.0, sq2, sq5,
        1.0, sq2, 2.0, sq5, sq5, 2.0, sq5, sq8,
        0.0, 1.0, 2.0, 3.0, sqa, 3.0, sqa, sqd
    };
    auto texture = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_R8UI, 8, 8, 0,
        GL_RED_INTEGER, GL_UNSIGNED_BYTE, image.data()
    );
    auto tex = distance_transform(texture, 8);
    REQUIRE_THROWS_AS(distance_transform(texture, -1), std::invalid_argument);
    REQUIRE_THROWS_AS(distance_transform(texture, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(distance_transform(texture, 9), std::invalid_argument);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    auto data = std::array<float, 8*8>{0};
    glGetTextureImage(
        tex, 0, GL_RED, GL_FLOAT, 8*8*sizeof(float), data.data()
    );

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            REQUIRE(data[x + y*8] == transform[x + y*8]);
        }
    }
}
