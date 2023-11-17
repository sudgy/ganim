#include <SFML/Window.hpp>
#include "ganim/gl/gl.hpp"

#include "ganim/scene/scene.hpp"
#include "ganim/object/shape.hpp"

#include "ganim/math.hpp"

using namespace ganim;

int main()
{
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, 1, 1);

    auto scene = Scene("Test.mp4", 256, 144, 16, 9, 30);
    auto obj = Shape(
        {{ 2,  2, 0, 1, 0, 0, 1},
         { 2, -2, 0, 1, 0, 0, 1},
         {-2, -2, 0, 1, 0, 0, 1},
         {-2,  2, 0, 1, 0, 0, 1}},
        {0, 1, 2, 0, 2, 3}
    );
    auto obj2 = Shape(
        {{ 2,  2, 0, 0, 1, 0, 1},
         { 2, -2, 0, 0, 1, 0, 1},
         {-2, -2, 0, 0, 1, 0, 1},
         {-2,  2, 0, 0, 1, 0, 1}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(obj);
    scene.add(obj2);
    using namespace pga3;
    obj2.shift(-2*e1 - 2*e3);
    for (int i = 0; i < 120; ++i) {
        obj2.shift(4*e3/120.0);
        scene.frame_advance();
    }
}
