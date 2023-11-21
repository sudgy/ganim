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

    auto scene = Scene("Test.mp4", 2560, 1440, 16, 9, 60);
    std::array<Shape, 6> shapes {
        Shape(
            {{ 2,  2, 0, 1, 0, 0},
             { 2, -2, 0, 1, 0, 0},
             {-2, -2, 0, 1, 0.25, 0},
             {-2,  2, 0, 1, 0.25, 0}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2,  2, -4, 1, 0, 0},
             { 2, -2, -4, 1, 0, 0},
             {-2, -2, -4, 1, 0.25, 0},
             {-2,  2, -4, 1, 0.25, 0}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2,  2,  0, 0, 1, 0},
             { 2, -2,  0, 0, 1, 0},
             { 2, -2, -4, 0, 1, 0.25},
             { 2,  2, -4, 0, 1, 0.25}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{-2,  2,  0, 0, 1, 0},
             {-2, -2,  0, 0, 1, 0},
             {-2, -2, -4, 0, 1, 0.25},
             {-2,  2, -4, 0, 1, 0.25}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2,  2, -4, 0, 0, 1},
             { 2,  2,  0, 0, 0, 1},
             {-2,  2,  0, 0.25, 0, 1},
             {-2,  2, -4, 0.25, 0, 1}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2, -2, -4, 0, 0, 1},
             { 2, -2,  0, 0, 0, 1},
             {-2, -2,  0, 0.25, 0, 1},
             {-2, -2, -4, 0.25, 0, 1}},
            {0, 1, 2, 0, 2, 3}
        )
    };
    using namespace pga3;
    scene.add(shapes[0]);
    shapes[0].set_opacity(0);
    shapes[0].animate().set_opacity(1);
    scene.wait(2);
    shapes[0].animate().rotate(e23, τ/2).shift(2*e1);
    scene.wait(2);
    shapes[0].animate().shift(-2*e1);
    scene.wait(2);
    shapes[2].shift(2*e1);
    shapes[3].shift(-2*e1);
    shapes[4].shift(2*e2);
    shapes[5].shift(-2*e2);
    for (int i = 1; i < 6; ++i) {
        scene.add(shapes[i]);
        shapes[i].set_opacity(0);
        shapes[i].animate().set_opacity(1);
    }
    scene.wait(2);
    shapes[2].animate().shift(-2*e1);
    shapes[3].animate().shift(2*e1);
    shapes[4].animate().shift(-2*e2);
    shapes[5].animate().shift(2*e2);
    scene.wait(1);
    for (int t = 0; t < 400; ++t) {
        for (int i = 0; i < 6; ++i) {
            shapes[i].rotate(e12 + 0.5*e13 + 0.2*e23, 0.05);
        }
        scene.frame_advance();
    }
}
