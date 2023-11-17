#include <SFML/Window.hpp>
#include "ganim/gl/gl.hpp"

#include "ganim/scene/scene.hpp"
#include "ganim/object/shape.hpp"

using namespace ganim;

int main()
{
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, 1, 1);

    auto scene = Scene("Test.mp4", 2560, 1440, 16, 9, 15);
    auto obj = Shape(
        {{ 2,  2, -65000},
         { 2, -2, -65000},
         {-2, -2, -65000},
         {-2,  2, -65000}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(obj);
    for (int i = 0; i < 30; ++i) {
        scene.frame_advance();
    }
}
