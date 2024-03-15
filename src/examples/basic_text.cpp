#include "ganim.hpp"

using namespace ganim;

void basic_text()
{
    auto scene = Scene("basic_text.mp4", 1920, 1080, 16, 9, 60);
    auto text = Text("Hello, world!");
    scene.add(text);
    text.set_visible(true);
    scene.wait(1);
}
