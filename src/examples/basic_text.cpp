#include "ganim.hpp"

using namespace ganim;

void basic_text()
{
    auto scene = Scene("basic_text.mp4", 2560, 1440, 16, 9, 60);
    scene.set_background_color(Color("#808080"));
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
    text.set_draw_subobject_ratio(0.1);
    text.set_outline(Color("#000000"), 0.04);
    noise_create(text, 0.25, {.duration = 2});
    scene.wait(3);
}
