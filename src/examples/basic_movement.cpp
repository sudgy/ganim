#include "ganim.hpp"

using namespace ganim;

void basic_movement()
{
    auto scene = Scene("basic_movement.mp4", 2560, 1440, 16, 9, 60);
    std::array<ObjectPtr<Shape>, 6> shapes {
        make_shape(
            {{ 2,  2, 0, 0, 1, 0, 0},
             { 2, -2, 0, 0, 1, 0, 0},
             {-2, -2, 0, 0, 1, 0.25, 0},
             {-2,  2, 0, 0, 1, 0.25, 0}},
             {0, 1, 2, 0, 2, 3}
        ),
        make_shape(
            {{ 2,  2, -4, 0, 1, 0, 0},
             { 2, -2, -4, 0, 1, 0, 0},
             {-2, -2, -4, 0, 1, 0.25, 0},
             {-2,  2, -4, 0, 1, 0.25, 0}},
             {0, 1, 2, 0, 2, 3}
        ),
        make_shape(
            {{ 2,  2,  0, 0, 0, 1, 0},
             { 2, -2,  0, 0, 0, 1, 0},
             { 2, -2, -4, 0, 0, 1, 0.25},
             { 2,  2, -4, 0, 0, 1, 0.25}},
             {0, 1, 2, 0, 2, 3}
        ),
        make_shape(
            {{-2,  2,  0, 0, 0, 1, 0},
             {-2, -2,  0, 0, 0, 1, 0},
             {-2, -2, -4, 0, 0, 1, 0.25},
             {-2,  2, -4, 0, 0, 1, 0.25}},
             {0, 1, 2, 0, 2, 3}
        ),
        make_shape(
            {{ 2,  2, -4, 0, 0, 0, 1},
             { 2,  2,  0, 0, 0, 0, 1},
             {-2,  2,  0, 0, 0.25, 0, 1},
             {-2,  2, -4, 0, 0.25, 0, 1}},
             {0, 1, 2, 0, 2, 3}
        ),
        make_shape(
            {{ 2, -2, -4, 0, 0, 0, 1},
             { 2, -2,  0, 0, 0, 0, 1},
             {-2, -2,  0, 0, 0.25, 0, 1},
             {-2, -2, -4, 0, 0.25, 0, 1}},
             {0, 1, 2, 0, 2, 3}
        )
    };
    using namespace pga3;
    scene.add(shapes[0]);
    fade_in(scene, shapes[0]);
    scene.wait(2);
    animate(scene, shapes[0]).rotate(τ/2, e23).shift(2*e1);
    scene.wait(2);
    animate(scene, shapes[0]).shift(-2*e1);
    scene.wait(2);
    shapes[2]->shift(2*e1);
    shapes[3]->shift(-2*e1);
    shapes[4]->shift(2*e2);
    shapes[5]->shift(-2*e2);
    for (int i = 1; i < 6; ++i) {
        scene.add(shapes[i]);
        fade_in(scene, shapes[i]);
    }
    scene.wait(2);
    animate(scene, shapes[2]).shift(-2*e1);
    animate(scene, shapes[3]).shift(2*e1);
    animate(scene, shapes[4]).shift(-2*e2);
    animate(scene, shapes[5]).shift(2*e2);
    scene.wait(1);
    for (int i = 0; i < 6; ++i) {
        shapes[i]->add_updater([&, i]{
            shapes[i]->rotate(0.05, e12 + 0.5*e13 + 0.2*e23);
        });
    }
    scene.wait(5);
}
