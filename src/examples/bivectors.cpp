#include "ganim.hpp"

#include <random>
#include "ganim/util/triangulation.hpp"

using namespace ganim;

void bivectors()
{
    //auto scene = Scene("bivectors.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("bivectors.mp4", 2560, 1440, 16, 9, 60);

    using namespace vga2;

    auto b1 = make_bivector(4*e12);
    b1->set_color(Color("FFFF00"));
    b1->shift(-4*e1 + e2);
    scene.add(b1);
    create_bivector(scene, b1);
    scene.wait(1);

    auto b2 = make_bivector(e1 + 2*e2, 2*e1);
    b2->set_color(Color("00FFFF"));
    b2->shift(e1 + e2);
    scene.add(b2);
    create_bivector(scene, b2);
    scene.wait(1);

    auto path1 = std::vector<Vec>();
    for (int i = 0; i < 128; ++i) {
        path1.push_back(e1*ga_exp(e12*i*τ/128));
    }
    auto b3 = make_bivector(path1);
    b3->set_color(Color("00FF00"));
    b3->shift(-3*e1 - 2*e2);
    scene.add(b3);
    create_bivector(scene, b3);
    scene.wait(1);

    auto path2 = std::vector<Vec>();
    for (int i = 0; i < 128; ++i) {
        path2.push_back(i*3.0/128*e1 + (-1 + std::cos(i*τ/128))*e2/2);
    }
    for (int i = 0; i < 128; ++i) {
        path2.push_back(3*e1*ga_exp(-e12*i*τ/512));
    }
    auto b4 = make_bivector(path2);
    b4->set_color(Color("FF00FF"));
    b4->shift(e1 - 0.5*e2);
    scene.add(b4);
    create_bivector(scene, b4);
    scene.wait(2);
}
