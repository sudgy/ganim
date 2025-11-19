#include "ganim.hpp"

using namespace ganim;

void video()
{
    //auto scene = Scene("video.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("video.mp4", 2560, 1440, 16, 9, 60);

    auto object = make_video("basic_text.mp4");
    object->scale(0.5);
    scene.add(object);
    scene.wait(1);
    object->set_visible(true);
    object->add_updater([&]{object->rotate(0.02, pga3::e13);});
    scene.wait(4);
}
