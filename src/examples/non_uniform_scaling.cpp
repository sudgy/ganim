#include "ganim.hpp"

using namespace ganim;

void non_uniform_scaling()
{
    using namespace vga3;
    auto scene = Scene("non_uniform_scaling.mp4", 2560, 1440, 16, 9, 60);
    auto obj1 = make_text("Test");
    auto obj2 = make_text("Test");
    auto obj3 = make_text("Test");
    obj1->shift(-4*e1);
    obj3->shift(4*e1);
    obj1->set_visible(true);
    obj2->set_visible(true);
    obj3->set_visible(true);
    scene.add(obj1, obj2, obj3);
    scene.wait(1);
    animate(scene, obj1).set_squish(0.5, e1, -4*e1);
    scene.wait(1.5);
    animate(scene, obj2).set_squish(1.5, e2, 0*e1);
    scene.wait(1.5);
    animate(scene, obj3).set_squish(0.5, e1 + e2, 4*e1);
    scene.wait(2);
}
