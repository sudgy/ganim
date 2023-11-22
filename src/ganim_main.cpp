#include <SFML/Window.hpp>
#include "ganim/gl/gl.hpp"

#include "ganim/scene/scene.hpp"
#include "ganim/object/shape.hpp"
#include "ganim/animation/creation.hpp"

#include "ganim/math.hpp"

using namespace ganim;

int main()
{
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, 1, 1);

    auto scene = Scene("Test.mp4", 2560, 1440, 16, 9, 60);
    std::array shapes {
        Shape(
            {{ 0,  0, 0, 0, 1, 1, 0},
             { 1,  1, 0, 1, 1, 1, 0},
             { 1, -1, 0, 1, 1, 1, 0},
             {-1, -1, 0, 1, 1, 1, 0},
             {-1,  1, 0, 1, 1, 1, 0}},
            {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1}
        ),
        Shape(
            {{ 0,  0, 0, 1, 1, 1, 0},
             { 1,  1, 0, 0, 1, 1, 0},
             { 1, -1, 0, 0, 1, 1, 0},
             {-1, -1, 0, 0, 1, 1, 0},
             {-1,  1, 0, 0, 1, 1, 0}},
            {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1}
        ),
        Shape(
            {{ 0,  0, 0, 0, 1, 1, 0},
             { 1,  1, 0, 1, 1, 1, 0},
             { 1, -1, 0, 2, 1, 1, 0},
             {-1, -1, 0, 3, 1, 1, 0},
             {-1,  1, 0, 4, 1, 1, 0}},
            {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1}
        ),
        Shape(
            {{ 0,  0, 0, 1, 1, 1, 0},
             { 1,  1, 0, 0, 1, 1, 0},
             { 1, -1, 0, 0, 1, 1, 0},
             {-1, -1, 0, 2, 1, 1, 0},
             {-1,  1, 0, 2, 1, 1, 0}},
            {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1}
        ),
        Shape(
            {{ 0,  0, 0, 1, 1, 1, 0},
             { 1,  1, 0, 1, 1, 1, 0},
             { 1, -1, 0, 0, 1, 1, 0},
             {-1, -1, 0, 1, 1, 1, 0},
             {-1,  1, 0, 0, 1, 1, 0}},
            {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1}
        ),
        Shape(
            {{ 0,  0, 0, 1, 1, 1, 0},
             { 1,  1, 0, 0, 1, 1, 0},
             { 1, -1, 0, 2, 1, 1, 0},
             {-1, -1, 0, 0, 1, 1, 0},
             {-1,  1, 0, 2, 1, 1, 0}},
            {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1}
        ),
    };
    using namespace vga2;
    shapes[0].shift(-3*e1 + 1.5*e2);
    shapes[1].shift(        1.5*e2);
    shapes[2].shift( 3*e1 + 1.5*e2);
    shapes[3].shift(-3*e1 - 1.5*e2);
    shapes[4].shift(      - 1.5*e2);
    shapes[5].shift( 3*e1 - 1.5*e2);
    for (auto& shape : shapes) {
        scene.add(shape);
        create(shape);
        scene.wait();
    }
    scene.wait();
}
