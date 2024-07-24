#include "ganim.hpp"

using namespace ganim;

void transforms()
{
    //auto scene = Scene("transforms.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("transforms.mp4", 2560, 1440, 16, 9, 60);

    //auto text1 = Text("1");
    //auto text2 = Text("2");
    //scene.add(text1, text2);
    //text1.set_visible(true);
    //scene.wait(1);
    //texture_transform(scene, text1, text2);
    //scene.wait(2);

    auto text = std::vector<Text>();
    for (int i = 0; i < 11; ++i) {
        text.emplace_back(std::to_string(i));
        text.back().scale(2);
    }
    scene.add(text);
    text[0].set_visible(true);
    scene.wait(1);
    for (int i = 0; i < 10; ++i) {
        texture_transform(scene, text[i], text[i+1]);
        scene.wait(1.5);
    }
    scene.wait(0.5);
}
