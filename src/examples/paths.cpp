#include "ganim.hpp"

using namespace ganim;

void paths()
{
    //auto scene = Scene("paths.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("paths.mp4", 2560, 1440, 16, 9, 60);
    using namespace vga2;
    auto square = make_path(
        {
            -e1 - e2,
             e1 - e2,
             e1 + e2,
            -e1 + e2
        },
        true
    );
    square->set_color("FFFF00");
    square->shift(-3*e1 - 1.5*e2);
    scene.add(square);
    create(scene, square);
    scene.wait(2);
    auto circle_points = std::vector<vga2::Vec>();
    for (int i = 0; i < 1000; ++i) {
        circle_points.push_back(std::cos(i*τ/1000)*e1 + std::sin(i*τ/1000)*e2);
    }
    auto circle = make_path(circle_points, true);
    circle->set_color("00FFFF");
    circle->shift(3*e1 - 1.5*e2);
    scene.add(circle);
    create(scene, circle);
    scene.wait(2);
    auto sin_points = std::vector<vga2::Vec>();
    for (int i = 0; i < 1000; ++i) {
        sin_points.push_back(i/100.0*e1 + std::cos(i/50.0)*e2);
    }
    auto sin = make_path(sin_points);
    sin->set_color("00FF00");
    sin->shift(-5*e1 + 1.5*e2);
    scene.add(sin);
    create(scene, sin);
    scene.wait(2);
}
