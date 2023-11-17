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

    auto scene = Scene("Test.mp4", 2560, 1440, 16, 9, 30);
    auto obj = Shape(
        {{ 2,  2, 0},
         { 2, -2, 0},
         {-2, -2, 0},
         {-2,  2, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    scene.add(obj);
    using namespace pga3;
    for (int i = 0; i < 120; ++i) {
        auto line = ((2*e1 + e0) ^ (2*e2 + e0)).dual().normalized();
        scene.get_camera().rotate(line, τ/120);
        scene.frame_advance();
    }
}
