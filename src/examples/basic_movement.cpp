#include <SFML/Window.hpp>
#include "ganim.hpp"

using namespace ganim;

void basic_movement()
{
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, 1, 1);

    auto scene = Scene("basic_movement.mp4", 2560, 1440, 16, 9, 60);
    std::array<Shape, 6> shapes {
        Shape(
            {{ 2,  2, 0, 0, 1, 0, 0},
             { 2, -2, 0, 0, 1, 0, 0},
             {-2, -2, 0, 0, 1, 0.25, 0},
             {-2,  2, 0, 0, 1, 0.25, 0}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2,  2, -4, 0, 1, 0, 0},
             { 2, -2, -4, 0, 1, 0, 0},
             {-2, -2, -4, 0, 1, 0.25, 0},
             {-2,  2, -4, 0, 1, 0.25, 0}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2,  2,  0, 0, 0, 1, 0},
             { 2, -2,  0, 0, 0, 1, 0},
             { 2, -2, -4, 0, 0, 1, 0.25},
             { 2,  2, -4, 0, 0, 1, 0.25}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{-2,  2,  0, 0, 0, 1, 0},
             {-2, -2,  0, 0, 0, 1, 0},
             {-2, -2, -4, 0, 0, 1, 0.25},
             {-2,  2, -4, 0, 0, 1, 0.25}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2,  2, -4, 0, 0, 0, 1},
             { 2,  2,  0, 0, 0, 0, 1},
             {-2,  2,  0, 0, 0.25, 0, 1},
             {-2,  2, -4, 0, 0.25, 0, 1}},
            {0, 1, 2, 0, 2, 3}
        ),
        Shape(
            {{ 2, -2, -4, 0, 0, 0, 1},
             { 2, -2,  0, 0, 0, 0, 1},
             {-2, -2,  0, 0, 0.25, 0, 1},
             {-2, -2, -4, 0, 0.25, 0, 1}},
            {0, 1, 2, 0, 2, 3}
        )
    };
    using namespace pga3;
    scene.add(shapes[0]);
    fade_in(shapes[0]);
    scene.wait(2);
    animate(shapes[0]).rotate(e23, τ/2).shift(2*e1);
    scene.wait(2);
    animate(shapes[0]).shift(-2*e1);
    scene.wait(2);
    shapes[2].shift(2*e1);
    shapes[3].shift(-2*e1);
    shapes[4].shift(2*e2);
    shapes[5].shift(-2*e2);
    for (int i = 1; i < 6; ++i) {
        scene.add(shapes[i]);
        fade_in(shapes[i]);
    }
    scene.wait(2);
    animate(shapes[2]).shift(-2*e1);
    animate(shapes[3]).shift(2*e1);
    animate(shapes[4]).shift(-2*e2);
    animate(shapes[5]).shift(2*e2);
    scene.wait(1);
    for (int i = 0; i < 6; ++i) {
        shapes[i].add_updater([&, i]{
            shapes[i].rotate(e12 + 0.5*e13 + 0.2*e23, 0.05);
        });
    }
    scene.wait(5);
}
