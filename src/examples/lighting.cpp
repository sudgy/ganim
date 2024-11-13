#include "ganim.hpp"

using namespace ganim;

void lighting()
{
    //auto scene = Scene("lighting.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("lighting.mp4", 2560, 1440, 16, 9, 60);
    auto cube = Shape(
        {
            {-1, -1, -1},
            {-1, -1,  1},
            {-1,  1, -1},
            {-1,  1,  1},
            { 1, -1, -1},
            { 1, -1,  1},
            { 1,  1, -1},
            { 1,  1,  1},
        },
        {
            0, 1, 2,  2, 1, 3,
            5, 4, 6,  5, 6, 7,
            1, 5, 3,  3, 5, 7,
            0, 2, 4,  4, 2, 6,
            0, 4, 1,  1, 4, 5,
            2, 3, 6,  6, 3, 7
        }
    );
    scene.add(cube);
    using namespace vga3;
    scene.get_camera().rotate(e23, 1).rotate(e12, 0.5);
    cube.set_visible(true);
    cube.set_color(Color("FFFF00"));
    cube.add_updater([&]{cube.rotate(e12, 0.01);});
    scene.wait(5);
}
