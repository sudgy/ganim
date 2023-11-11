#include <SFML/Window.hpp>
#include "ganim/gl/gl.hpp"

#include "ganim/scene/scene.hpp"

using namespace ganim;

int main()
{
    constexpr int width = 256;
    constexpr int height = 144;
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, width, height);

    auto scene = Scene("Test.mp4", 256, 144, 60);
    for (int i = 0; i < 120; ++i) {
        scene.frame_advance();
    }
}
