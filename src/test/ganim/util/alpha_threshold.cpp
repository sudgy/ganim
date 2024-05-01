#include <catch2/catch_test_macros.hpp>

#include "ganim/util/alpha_threshold.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim;

TEST_CASE("alpha_threshold", "[object]") {
    auto image = std::array<unsigned char, 8*8*4>{0};
    auto set_pixel = [&](int x, int y, unsigned char alpha) {
        image[(x + y*8)*4 + 3] = alpha;
    };
    set_pixel(2, 2, 64);
    set_pixel(4, 4, 192);
    set_pixel(6, 6, 255);
    auto texture = gl::Texture(image.data(), 8, 8);
    auto tex1 = alpha_threshold(texture, 0.5, 8, 8);
    auto tex2 = alpha_threshold(texture, 1.0, 8, 8);
    REQUIRE_THROWS_AS(alpha_threshold(texture,1, -1, 8), std::invalid_argument);
    REQUIRE_THROWS_AS(alpha_threshold(texture, 1, 8, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(alpha_threshold(texture, 1, 8, 9), std::invalid_argument);
    REQUIRE_THROWS_AS(alpha_threshold(texture, 1, 9, 8), std::invalid_argument);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    auto data1 = std::array<unsigned char, 8*8>{0};
    glGetTextureImage(
        tex1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 8*8, data1.data()
    );
    auto data2 = std::array<unsigned char, 8*8>{0};
    glGetTextureImage(
        tex2, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 8*8, data2.data()
    );

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            INFO("x = " << x << ", y = " << y);
            auto p1 = data1[x + y*8];
            auto p2 = data2[x + y*8];
            if (x == 4 and y == 4) {
                REQUIRE(static_cast<int>(p1) == 255);
                REQUIRE(static_cast<int>(p2) == 0);
            }
            else if (x == 6 and y == 6) {
                REQUIRE(static_cast<int>(p1) == 255);
                REQUIRE(static_cast<int>(p2) == 255);
            }
            else {
                REQUIRE(static_cast<int>(p1) == 0);
                REQUIRE(static_cast<int>(p2) == 0);
            }
        }
    }
}
