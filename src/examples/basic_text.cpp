#include "ganim.hpp"

using namespace ganim;

void basic_text()
{
    using namespace vga2;
    auto scene = Scene("basic_text.mp4", 2560, 1440, 16, 9, 60);
    scene.set_background_color(Color("#808080"));
    auto text = make_tex("\\int  _{-1}  ^1  \\sqrt{",
            "\\frac{1  -  x  ^2  }{  {1 \\over 4}}}",
            "\\,\\mathrm{d}  x  =  \\frac{\\tau  }{  2}");
    text->set_colors({
        {"x", "#FF0000"},
        {"\\frac{\\tau", "#00FF00"},
        {"\\int", "#808080"},
        {"\\sqrt{", "#FFFF00"},
        {"}{", "#0000FF"}
    });
    auto hello = make_text("Hello, world!\nA second line");
    hello->shift(3*e2);
    hello->set_visible(true);
    hello->set_outline(Color("#000000"), 0.04);
    scene.add(hello, text);
    text->set_draw_subobject_ratio(0.1);
    text->set_outline(Color("#000000"), 0.04);
    text->shift(-2*e2);
    noise_create(scene, text, 0.25, {.duration = 2});
    auto align1 = make_text("Aligning");
    auto align2 = make_text(
            {.font_filename = "fonts/NewCM10-Italic.otf"}, "weird");
    auto align3 = make_tex("\\text{ones}");
    align1->to_edge(*scene.get_camera(), -e2).shift(-2.5*e1);
    align2->shift(1*e1).align_to(*align1, -e2);
    align3->shift(3.5*e1).align_to(*align1, e2);
    align1->set_visible(true);
    align2->set_visible(true);
    align3->set_visible(true);
    scene.add(align1, align2, align3);
    scene.wait(3);
}
