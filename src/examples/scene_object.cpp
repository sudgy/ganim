#include "ganim.hpp"

using namespace ganim;
using namespace vga2;

void scene_object()
{
    auto scene = Scene("scene_object.mp4", 2560, 1440, 16, 9, 60);

    auto scene2 = ObjectPtr<SceneObject>(320, 320, 2, 2, 60);
    scene2->set_background_color("80808080");
    auto obj1 = make_text("Hello");
    auto obj2 = make_text("World");
    scene.add(obj1);
    scene2->add(obj2);
    scene.add(scene2);
    obj1->set_visible(true);
    obj2->set_visible(true);
    scene2->set_visible(true);
    scene.wait(1);
    animate(scene, scene2).shift(2*e1);
    scene.wait(2);
    animate(scene2->get_scene(), obj2).shift(2*e1);
    scene.wait(2);
}
