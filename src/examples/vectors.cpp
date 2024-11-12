#include "ganim.hpp"

using namespace ganim;

std::vector<Path> get_coordinate_grid()
{
    using namespace vga2;
    auto result = std::vector<Path>();
    for (int x = -10; x <= 10; ++x) {
        if (x != 0) {
            result.push_back(Path({x*e1 - 10*e2, x*e1 + 10*e2}, false, 0.03));
            result.back().set_color("606060");
        }
    }
    for (int y = -10; y <= 10; ++y) {
        if (y != 0) {
            result.push_back(Path({y*e2 - 10*e1, y*e2 + 10*e1}, false, 0.03));
            result.back().set_color("606060");
        }
    }
    result.push_back(Path({-10*e1, 10*e1}, false, 0.03));
    result.back().shift(0.001*vga3::e3);
    result.push_back(Path({-10*e2, 10*e2}, false, 0.03));
    result.back().shift(0.001*vga3::e3);
    return result;
}

void vectors()
{
    //auto scene = Scene("vectors.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("vectors.mp4", 2560, 1440, 16, 9, 60);

    using namespace vga3;
    auto bad_coordinate_grid_base = get_coordinate_grid();
    auto bad_coordinate_grid = Group(bad_coordinate_grid_base);
    scene.add(bad_coordinate_grid);
    bad_coordinate_grid.set_visible(true);

    auto vector = ganim::Vector(3*e1 + 2*e2);
    vector.set_color(Color("FFFF00"));
    scene.add(vector);
    create(scene, vector);
    scene.wait(2);
    animate(scene, vector).rotate(e12, τ/4);
    scene.wait(2);
    animate(scene, vector).scale(-1);
    scene.wait(2);
    animate(scene, scene.get_camera()).rotate(e23, 1).rotate(e12, 0.5);
    animate(scene, vector).set_end(e1 + 2*e3);
    scene.wait(2);
    auto t = Value(0.0);
    vector.add_updater([&]{
        vector.set_end(e1 + 2*std::cos(t())*e3 + 2*std::sin(t())*e2);
    });
    animate(scene, t, {.duration = 2}).set_value(τ);
    scene.wait(3);
}
