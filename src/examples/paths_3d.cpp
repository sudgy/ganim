#include "ganim.hpp"

using namespace ganim;

void paths_3d()
{
    //auto scene = Scene("paths_3d.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("paths_3d.mp4", 2560, 1440, 16, 9, 60);
    using namespace vga3;
    auto helix_points = std::vector<vga3::Vec>();
    helix_points.reserve(4096);
    for (int i = 0; i < 4096; ++i) {
        helix_points.push_back(
            2*std::cos(i/128.0)*e1 + 2*std::sin(i/128.0)*e2 + (i/1024.0 - 2) * e3
        );
    }
    auto helix = make_path(helix_points, 16, false, 0.1);
    helix->set_color("FF0000");
    auto cam = scene.get_camera();
    cam->rotate(τ/6, e23);
    create(scene, helix);
    scene.wait(2);
    auto torus_points = std::vector<vga3::Vec>();
    torus_points.reserve(128);
    for (int i = 0; i < 128; ++i) {
        torus_points.push_back(
            4*std::cos(τ*i/128.0)*e1 + 4*std::sin(τ*i/128.0)*e2
        );
    }
    auto torus = make_path(torus_points, 32, true, 1);
    torus->set_color("00FF00");
    create(scene, torus);
    scene.wait(2);
}
