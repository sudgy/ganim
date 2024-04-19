#include "ganim.hpp"

using namespace ganim;

void basic_text()
{
    auto scene = Scene("basic_text.mp4", 2560, 1440, 16, 9, 60);
    auto text = Tex("\\int", "_{-1}", "^1", "\\sqrt{",
            "\\frac{1", "-", "x", "^2", "}{", "{1 \\over 4}}}",
            "\\,\\mathrm{d}", "x", "=", "\\frac{\\tau", "}{", "2}");
    text.set_colors({
        {"x", "#FF0000"},
        {"\\frac{\\tau", "#00FF00"},
        {"\\int", "#808080"},
        {"\\sqrt{", "#FFFF00"},
        {"}{", "#0000FF"}
    });
    scene.add(text);
    text.set_visible(true);
    scene.wait(1);
}
