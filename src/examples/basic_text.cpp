#include "ganim.hpp"

using namespace ganim;

void basic_text()
{
    auto scene = Scene("basic_text.mp4", 2560, 1440, 16, 9, 60);
    auto text = Tex("\1\\int", "_{-1}", "^1", "\1\\sqrt{", "\1\\frac{1", "-", "x",
            "^2", "\1}{", "{1 \\over 4}}}", "=", "\1\\frac{\\tau", "\1}{", "2}");
    scene.add(text);
    text.set_visible(true);
    scene.wait(1);
}
