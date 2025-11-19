#include "ganim.hpp"

using namespace ganim;

void video()
{
    auto scene = Scene("test_video.mp4", 64, 64, 2, 2, 4);
    using namespace vga2;
    auto shape1 = make_polygon_shape({0*e1, e1, e1 + e2, e2});
    auto shape2 = shape1->polymorphic_copy();
    auto shape3 = shape1->polymorphic_copy();
    auto shape4 = shape1->polymorphic_copy();
    shape1->shift(-e1);
    shape3->shift(-e1 - e2);
    shape4->shift(-e2);
    shape1->set_color("FF0000");
    shape2->set_color("00FF00");
    shape3->set_color("0000FF");
    shape4->set_color("FFFFFF");
    shape1->set_visible(true);
    shape2->set_visible(true);
    shape3->set_visible(true);
    shape4->set_visible(true);
    scene.add(shape1, shape2, shape3, shape4);
    scene.frame_advance();
    scene.frame_advance();
    shape1->set_color("FFFF00");
    shape2->set_color("00FFFF");
    shape3->set_color("FF00FF");
    shape4->set_color("000000");
    scene.frame_advance();
    scene.frame_advance();
    shape1->set_color("800000");
    shape2->set_color("008000");
    shape3->set_color("000080");
    shape4->set_color("808080");
    scene.frame_advance();
    shape1->set_color("808000");
    shape2->set_color("008080");
    shape3->set_color("800080");
    shape4->set_color("000000");
    scene.frame_advance();
    // I don't seem to be catching the final frame, which might be fine?  For
    // now I'll just do this to make a fake final frame
    shape1->set_color("000000");
    shape2->set_color("000000");
    shape3->set_color("000000");
    shape4->set_color("000000");
    scene.frame_advance();
#if 0
    auto scene = Scene("video.mp4", 854, 480, 16, 9, 60);
    //auto scene = Scene("video.mp4", 2560, 1440, 16, 9, 60);

    auto object = make_video("basic_text.mp4");
    object->scale(0.5);
    scene.add(object);
    scene.wait(1);
    object->set_visible(true);
    object->add_updater([&]{object->rotate(0.02, pga3::e13);});
    scene.wait(4);
#endif
}
