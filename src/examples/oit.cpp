#include "ganim.hpp"

using namespace ganim;

void oit()
{
    //auto scene = Scene("oit.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("oit.mp4", 2560, 1440, 16, 9, 60);

    scene.set_transparency_layers(2);

    auto square1 = Shape(
        {{ 2,  2},
         { 2, -2},
         {-2, -2},
         {-2,  2}},
        {0, 1, 2, 0, 2, 3}
    );
    auto square2 = Shape(
        {{ 2,  2},
         { 2, -2},
         {-2, -2},
         {-2,  2}},
        {0, 1, 2, 0, 2, 3}
    );
    square1.set_color("FFFF00");
    square2.set_color("00FFFF");
    square1.set_opacity(0.75);
    square2.set_opacity(0.75);
    square1.set_visible(true);
    square2.set_visible(true);
    scene.add(square1, square2);
    square2.add_updater([&]{
        square2.rotate(0.02, pga3::e23);
    });
    scene.wait(10);
}
