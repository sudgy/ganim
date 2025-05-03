#include "ganim.hpp"

using namespace ganim;

void polyhedra()
{
    //auto scene = Scene("polyhedra.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("polyhedra.mp4", 2560, 1440, 16, 9, 60);
    using namespace vga3;
    scene.set_transparency_layers(4);
    auto cam = scene.get_camera();
    cam->rotate(τ/6, e23);
    cam->rotate(0.2, e12);
    auto cube = make_polyhedron(
        {
            -2*e1 - 2*e2 - 2*e3,
             2*e1 - 2*e2 - 2*e3,
            -2*e1 + 2*e2 - 2*e3,
             2*e1 + 2*e2 - 2*e3,
            -2*e1 - 2*e2 + 2*e3,
             2*e1 - 2*e2 + 2*e3,
            -2*e1 + 2*e2 + 2*e3,
             2*e1 + 2*e2 + 2*e3
        },
        {
            {0, 2, 3, 1},
            {0, 1, 5, 4},
            {1, 3, 7, 5},
            {0, 4, 6, 2},
            {2, 6, 7, 3},
            {4, 5, 7, 6}
        }
    );
    cube->set_color("00FFFF");
    cube->get_edges()->set_color("FF0000");
    cube->set_draw_subobject_ratio(0.1);
    cube->get_edges()->set_draw_subobject_ratio(0.1);
    cube->get_faces()->set_draw_subobject_ratio(0.1);
    create(scene, cube);
    scene.wait(2);
    cube->add_updater([&]{cube->rotate(0.03, e13 + e23);});
    scene.wait(5);
}
