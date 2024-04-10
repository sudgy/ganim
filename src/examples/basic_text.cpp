#include "ganim.hpp"

using namespace ganim;

void basic_text()
{
    auto scene = Scene("basic_text.mp4", 2560, 1440, 16, 9, 60);
    auto text = Tex("\1\\int", "_{-1}", "^1", "\1\\sqrt{",
            "\1\\frac{1", "-", "x", "^2", "\1}{", "{1 \\over 4}}}",
            "\1\\,\\mathrm{d}", "x", "=", "\1\\frac{\\tau", "\1}{", "2}");
    text.set_colors({
        {"x", "#FF0000"},
        {"\\tau", "#00FF00"},
        {"\1\\int", "#808080"},
        {"\1\\sqrt{", "#FFFF00"},
        {"\1}{", "#0000FF"}
    });
    scene.add(text);
    text.set_visible(true);
    scene.wait(1);
}
