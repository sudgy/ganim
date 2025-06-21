#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/render_math_list.hpp"

using namespace ganim;
using namespace ganim::gex;

TEST_CASE("GeX render_math_list basic spacing", "[object][text][gex]") {
    // a + b = c
    // +a+
    // a + +b
    // a+=+b
    using enum AtomType;
    auto add_to_list = [&](
        MathList& list,
        AtomType type,
        std::uint32_t codepoint,
        int group
    )
    {
        list.emplace_back(
            Atom(
                Box(),
                type,
                AtomField(
                    AtomFieldSymbol(codepoint),
                    Box()
                )
            ),
            group,
            0
        );
    };
    auto list1 = MathList();
    add_to_list(list1, Ord, U'a', 0);
    add_to_list(list1, Bin, U'+', 1);
    add_to_list(list1, Ord, U'b', 2);
    add_to_list(list1, Rel, U'=', 3);
    add_to_list(list1, Ord, U'c', 4);
    auto box1 = render_math_list(list1, Style::Text);
    auto list2 = MathList();
    add_to_list(list2, Bin, U'+', 0);
    add_to_list(list2, Ord, U'a', 1);
    add_to_list(list2, Bin, U'+', 2);
    auto box2 = render_math_list(list2, Style::Text);
    auto list3 = MathList();
    add_to_list(list3, Ord, U'a', 0);
    add_to_list(list3, Bin, U'+', 1);
    add_to_list(list3, Bin, U'+', 2);
    add_to_list(list3, Ord, U'b', 3);
    auto box3 = render_math_list(list3, Style::Text);
    auto list4 = MathList();
    add_to_list(list4, Ord, U'a', 0);
    add_to_list(list4, Bin, U'+', 1);
    add_to_list(list4, Rel, U'=', 2);
    add_to_list(list4, Bin, U'+', 3);
    add_to_list(list4, Ord, U'b', 4);
    auto box4 = render_math_list(list4, Style::Text);

    REQUIRE(box1.height == 0.703125);
    REQUIRE(box1.depth == 0.09375);
    REQUIRE(box1.width == 3.85625);
    REQUIRE(box1.glyphs.size() == 5);
    REQUIRE(box1.glyphs[0].x_pos == 0);
    REQUIRE(box1.glyphs[0].y_pos == 0);
    REQUIRE(box1.glyphs[0].draw_x == 0.03125);
    REQUIRE(box1.glyphs[0].draw_y == 0.453125);
    REQUIRE(box1.glyphs[0].width == 0.4765625);
    REQUIRE(box1.glyphs[0].height == 0.46875);
    REQUIRE(box1.glyphs[0].y_min == -0.2578125);
    REQUIRE(box1.glyphs[0].y_max == 0.75);
    REQUIRE(box1.glyphs[0].texture_width == 0.059570312f);
    REQUIRE(box1.glyphs[0].texture_height == 0.05859375f);
    REQUIRE(box1.glyphs[0].group_index == 0);
    REQUIRE(box1.glyphs[1].x_pos == 0.74322916666666661);
    REQUIRE(box1.glyphs[1].y_pos == 0);
    REQUIRE(box1.glyphs[1].draw_x == 0.79010416666666661);
    REQUIRE(box1.glyphs[1].draw_y == 0.6015625);
    REQUIRE(box1.glyphs[1].width == 0.6875);
    REQUIRE(box1.glyphs[1].height == 0.6953125);
    REQUIRE(box1.glyphs[1].y_min == -0.2578125);
    REQUIRE(box1.glyphs[1].y_max == 0.75);
    REQUIRE(box1.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box1.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box1.glyphs[1].group_index == 1);
    REQUIRE(box1.glyphs[2].x_pos == 1.69739583333333321);
    REQUIRE(box1.glyphs[2].y_pos == 0);
    REQUIRE(box1.glyphs[2].draw_x == 1.73645833333333321);
    REQUIRE(box1.glyphs[2].draw_y == 0.703125);
    REQUIRE(box1.glyphs[2].width == 0.390625);
    REQUIRE(box1.glyphs[2].height == 0.71875);
    REQUIRE(box1.glyphs[2].y_min == -0.2578125);
    REQUIRE(box1.glyphs[2].y_max == 0.75);
    REQUIRE(box1.glyphs[2].texture_width == 0.048828125f);
    REQUIRE(box1.glyphs[2].texture_height == 0.08984375f);
    REQUIRE(box1.glyphs[2].group_index == 2);
    REQUIRE(box1.glyphs[3].x_pos == 2.4213541666666667);
    REQUIRE(box1.glyphs[3].y_pos == 0);
    REQUIRE(box1.glyphs[3].draw_x == 2.4682291666666667);
    REQUIRE(box1.glyphs[3].draw_y == 0.375);
    REQUIRE(box1.glyphs[3].width == 0.6875);
    REQUIRE(box1.glyphs[3].height == 0.25);
    REQUIRE(box1.glyphs[3].y_min == -0.2578125);
    REQUIRE(box1.glyphs[3].y_max == 0.75);
    REQUIRE(box1.glyphs[3].texture_width == 0.0859375f);
    REQUIRE(box1.glyphs[3].texture_height == 0.03125f);
    REQUIRE(box1.glyphs[3].group_index == 3);
    REQUIRE(box1.glyphs[4].x_pos == 3.44218750000000018);
    REQUIRE(box1.glyphs[4].y_pos == 0);
    REQUIRE(box1.glyphs[4].draw_x == 3.47343750000000018);
    REQUIRE(box1.glyphs[4].draw_y == 0.453125);
    REQUIRE(box1.glyphs[4].width == 0.4140625);
    REQUIRE(box1.glyphs[4].height == 0.46875);
    REQUIRE(box1.glyphs[4].y_min == -0.2578125);
    REQUIRE(box1.glyphs[4].y_max == 0.75);
    REQUIRE(box1.glyphs[4].texture_width == 0.051757812f );
    REQUIRE(box1.glyphs[4].texture_height == 0.05859375f);
    REQUIRE(box1.glyphs[4].group_index == 4);

    REQUIRE(box2.height == 0.6015625);
    REQUIRE(box2.depth == 0.09375);
    REQUIRE(box2.width == 1.9945068359375);
    REQUIRE(box2.glyphs.size() == 3);
    REQUIRE(box2.glyphs[0].x_pos == 0);
    REQUIRE(box2.glyphs[0].y_pos == 0);
    REQUIRE(box2.glyphs[0].draw_x == 0.046875);
    REQUIRE(box2.glyphs[0].draw_y == 0.6015625);
    REQUIRE(box2.glyphs[0].width == 0.6875);
    REQUIRE(box2.glyphs[0].height == 0.6953125);
    REQUIRE(box2.glyphs[0].y_min == -0.2578125);
    REQUIRE(box2.glyphs[0].y_max == 0.75);
    REQUIRE(box2.glyphs[0].texture_width == 0.0859375f);
    REQUIRE(box2.glyphs[0].texture_height == 0.086914062f);
    REQUIRE(box2.glyphs[0].group_index == 0);
    REQUIRE(box2.glyphs[1].x_pos == 0.7779541015625);
    REQUIRE(box2.glyphs[1].y_pos == 0);
    REQUIRE(box2.glyphs[1].draw_x == 0.8092041015625);
    REQUIRE(box2.glyphs[1].draw_y == 0.453125);
    REQUIRE(box2.glyphs[1].width == 0.4765625);
    REQUIRE(box2.glyphs[1].height == 0.46875);
    REQUIRE(box2.glyphs[1].y_min == -0.2578125);
    REQUIRE(box2.glyphs[1].y_max == 0.75);
    REQUIRE(box2.glyphs[1].texture_width == 0.059570312f);
    REQUIRE(box2.glyphs[1].texture_height == 0.05859375f);
    REQUIRE(box2.glyphs[1].group_index == 1);
    REQUIRE(box2.glyphs[2].x_pos == 1.3070068359375);
    REQUIRE(box2.glyphs[2].y_pos == 0);
    REQUIRE(box2.glyphs[2].draw_x == 1.3538818359375);
    REQUIRE(box2.glyphs[2].draw_y == 0.6015625);
    REQUIRE(box2.glyphs[2].width == 0.6875);
    REQUIRE(box2.glyphs[2].height == 0.6953125);
    REQUIRE(box2.glyphs[2].y_min == -0.2578125);
    REQUIRE(box2.glyphs[2].y_max == 0.75);
    REQUIRE(box2.glyphs[2].texture_width == 0.0859375f);
    REQUIRE(box2.glyphs[2].texture_height == 0.086914062f);
    REQUIRE(box2.glyphs[2].group_index == 2);

    REQUIRE(box3.height == 0.703125);
    REQUIRE(box3.depth == 0.09375);
    REQUIRE(box3.width == 2.86597493489583321);
    REQUIRE(box3.glyphs.size() == 4);
    REQUIRE(box3.glyphs[0].x_pos == 0);
    REQUIRE(box3.glyphs[0].y_pos == 0);
    REQUIRE(box3.glyphs[0].draw_x == 0.03125);
    REQUIRE(box3.glyphs[0].draw_y == 0.453125);
    REQUIRE(box3.glyphs[0].width == 0.4765625);
    REQUIRE(box3.glyphs[0].height == 0.46875);
    REQUIRE(box3.glyphs[0].y_min == -0.2578125);
    REQUIRE(box3.glyphs[0].y_max == 0.75);
    REQUIRE(box3.glyphs[0].texture_width == 0.059570312f);
    REQUIRE(box3.glyphs[0].texture_height == 0.05859375f);
    REQUIRE(box3.glyphs[0].group_index == 0);
    REQUIRE(box3.glyphs[1].x_pos == 0.74322916666666661);
    REQUIRE(box3.glyphs[1].y_pos == 0);
    REQUIRE(box3.glyphs[1].draw_x == 0.79010416666666661);
    REQUIRE(box3.glyphs[1].draw_y == 0.6015625);
    REQUIRE(box3.glyphs[1].width == 0.6875);
    REQUIRE(box3.glyphs[1].height == 0.6953125);
    REQUIRE(box3.glyphs[1].y_min == -0.2578125);
    REQUIRE(box3.glyphs[1].y_max == 0.75);
    REQUIRE(box3.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box3.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box3.glyphs[1].group_index == 1);
    REQUIRE(box3.glyphs[2].x_pos == 1.69739583333333321);
    REQUIRE(box3.glyphs[2].y_pos == 0);
    REQUIRE(box3.glyphs[2].draw_x == 1.74427083333333321);
    REQUIRE(box3.glyphs[2].draw_y == 0.6015625);
    REQUIRE(box3.glyphs[2].width == 0.6875);
    REQUIRE(box3.glyphs[2].height == 0.6953125);
    REQUIRE(box3.glyphs[2].y_min == -0.2578125);
    REQUIRE(box3.glyphs[2].y_max == 0.75);
    REQUIRE(box3.glyphs[2].texture_width == 0.0859375f);
    REQUIRE(box3.glyphs[2].texture_height == 0.086914062f);
    REQUIRE(box3.glyphs[2].group_index == 2);
    REQUIRE(box3.glyphs[3].x_pos == 2.47534993489583321);
    REQUIRE(box3.glyphs[3].y_pos == 0);
    REQUIRE(box3.glyphs[3].draw_x == 2.51441243489583321);
    REQUIRE(box3.glyphs[3].draw_y == 0.703125);
    REQUIRE(box3.glyphs[3].width == 0.390625);
    REQUIRE(box3.glyphs[3].height == 0.71875);
    REQUIRE(box3.glyphs[3].y_min == -0.2578125);
    REQUIRE(box3.glyphs[3].y_max == 0.75);
    REQUIRE(box3.glyphs[3].texture_width == 0.048828125f);
    REQUIRE(box3.glyphs[3].texture_height == 0.08984375f);
    REQUIRE(box3.glyphs[3].group_index == 3);

    REQUIRE(box4.height == 0.703125);
    REQUIRE(box4.depth == 0.09375);
    REQUIRE(box4.width == 3.73929850260416652);
    REQUIRE(box4.glyphs.size() == 5);
    REQUIRE(box4.glyphs[0].x_pos == 0);
    REQUIRE(box4.glyphs[0].y_pos == 0);
    REQUIRE(box4.glyphs[0].draw_x == 0.03125);
    REQUIRE(box4.glyphs[0].draw_y == 0.453125);
    REQUIRE(box4.glyphs[0].width == 0.4765625);
    REQUIRE(box4.glyphs[0].height == 0.46875);
    REQUIRE(box4.glyphs[0].y_min == -0.2578125);
    REQUIRE(box4.glyphs[0].y_max == 0.75);
    REQUIRE(box4.glyphs[0].texture_width == 0.059570312f);
    REQUIRE(box4.glyphs[0].texture_height == 0.05859375f);
    REQUIRE(box4.glyphs[0].group_index == 0);
    REQUIRE(box4.glyphs[1].x_pos == 0.529052734375);
    REQUIRE(box4.glyphs[1].y_pos == 0);
    REQUIRE(box4.glyphs[1].draw_x == 0.575927734375);
    REQUIRE(box4.glyphs[1].draw_y == 0.6015625);
    REQUIRE(box4.glyphs[1].width == 0.6875);
    REQUIRE(box4.glyphs[1].height == 0.6953125);
    REQUIRE(box4.glyphs[1].y_min == -0.2578125);
    REQUIRE(box4.glyphs[1].y_max == 0.75);
    REQUIRE(box4.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box4.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box4.glyphs[1].group_index == 1);
    REQUIRE(box4.glyphs[2].x_pos == 1.54988606770833326);
    REQUIRE(box4.glyphs[2].y_pos == 0);
    REQUIRE(box4.glyphs[2].draw_x == 1.59676106770833326);
    REQUIRE(box4.glyphs[2].draw_y == 0.375);
    REQUIRE(box4.glyphs[2].width == 0.6875);
    REQUIRE(box4.glyphs[2].height == 0.25);
    REQUIRE(box4.glyphs[2].y_min == -0.2578125);
    REQUIRE(box4.glyphs[2].y_max == 0.75);
    REQUIRE(box4.glyphs[2].texture_width == 0.0859375f);
    REQUIRE(box4.glyphs[2].texture_height == 0.03125f);
    REQUIRE(box4.glyphs[2].group_index == 2);
    REQUIRE(box4.glyphs[3].x_pos == 2.57071940104166652);
    REQUIRE(box4.glyphs[3].y_pos == 0);
    REQUIRE(box4.glyphs[3].draw_x == 2.61759440104166652);
    REQUIRE(box4.glyphs[3].draw_y == 0.6015625);
    REQUIRE(box4.glyphs[3].width == 0.6875);
    REQUIRE(box4.glyphs[3].height == 0.6953125);
    REQUIRE(box4.glyphs[3].y_min == -0.2578125);
    REQUIRE(box4.glyphs[3].y_max == 0.75);
    REQUIRE(box4.glyphs[3].texture_width == 0.0859375f);
    REQUIRE(box4.glyphs[3].texture_height == 0.086914062f);
    REQUIRE(box4.glyphs[3].group_index == 3);
    REQUIRE(box4.glyphs[4].x_pos == 3.34867350260416652);
    REQUIRE(box4.glyphs[4].y_pos == 0);
    REQUIRE(box4.glyphs[4].draw_x == 3.38773600260416652);
    REQUIRE(box4.glyphs[4].draw_y == 0.703125);
    REQUIRE(box4.glyphs[4].width == 0.390625);
    REQUIRE(box4.glyphs[4].height == 0.71875);
    REQUIRE(box4.glyphs[4].y_min == -0.2578125);
    REQUIRE(box4.glyphs[4].y_max == 0.75);
    REQUIRE(box4.glyphs[4].texture_width == 0.048828125f);
    REQUIRE(box4.glyphs[4].texture_height == 0.08984375f);
    REQUIRE(box4.glyphs[4].group_index == 4);
}

TEST_CASE("GeX render_math_list style changes", "[object][text][gex]") {
    auto list = MathList();
    auto add_to_list = [&](
        MathList& list,
        AtomType type,
        std::uint32_t codepoint,
        int group
    )
    {
        list.emplace_back(
            Atom(
                Box(),
                type,
                AtomField(
                    AtomFieldSymbol(codepoint),
                    Box()
                )
            ),
            group,
            0
        );
    };
    using enum AtomType;
    add_to_list(list, Ord, U'a', 0);
    list.emplace_back(CommandNoad("scriptstyle"), 1, 0);
    add_to_list(list, Ord, U'+', 1);
    list.emplace_back(CommandNoad("displaystyle"), 2, 0);
    add_to_list(list, Ord, U'a', 2);
    auto box = render_math_list(list, Style::Text);

    REQUIRE(box.height == 0.453125);
    REQUIRE(box.depth == 0.06562499999999999);
    REQUIRE(box.width == 1.48104166666666659);
    REQUIRE(box.glyphs.size() == 3);
    REQUIRE(box.glyphs[0].x_pos == 0);
    REQUIRE(box.glyphs[0].y_pos == 0);
    REQUIRE(box.glyphs[0].draw_x == 0.03125);
    REQUIRE(box.glyphs[0].draw_y == 0.453125);
    REQUIRE(box.glyphs[0].width == 0.4765625);
    REQUIRE(box.glyphs[0].height == 0.46875);
    REQUIRE(box.glyphs[0].y_min == -0.2578125);
    REQUIRE(box.glyphs[0].y_max == 0.75);
    REQUIRE(box.glyphs[0].texture_width == 0.059570312f);
    REQUIRE(box.glyphs[0].texture_height == 0.05859375f);
    REQUIRE(box.glyphs[0].group_index == 0);
    REQUIRE(box.glyphs[1].x_pos == 0.52322916666666663);
    REQUIRE(box.glyphs[1].y_pos == 0);
    REQUIRE(box.glyphs[1].draw_x == 0.55604166666666666);
    REQUIRE(box.glyphs[1].draw_y == 0.42109374999999999);
    REQUIRE(box.glyphs[1].width == 0.48124999999999996);
    REQUIRE(box.glyphs[1].height == 0.48671874999999998);
    REQUIRE(box.glyphs[1].y_min == -0.18046874999999998);
    REQUIRE(box.glyphs[1].y_max == 0.52499999999999991);
    REQUIRE(box.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box.glyphs[1].group_index == 1);
    REQUIRE(box.glyphs[2].x_pos == 1.00447916666666659);
    REQUIRE(box.glyphs[2].y_pos == 0);
    REQUIRE(box.glyphs[2].draw_x == 1.03572916666666659);
    REQUIRE(box.glyphs[2].draw_y == 0.453125);
    REQUIRE(box.glyphs[2].width == 0.4765625);
    REQUIRE(box.glyphs[2].height == 0.46875);
    REQUIRE(box.glyphs[2].y_min == -0.2578125);
    REQUIRE(box.glyphs[2].y_max == 0.75);
    REQUIRE(box.glyphs[2].texture_width == 0.059570312f);
    REQUIRE(box.glyphs[2].texture_height == 0.05859375f);
    REQUIRE(box.glyphs[2].group_index == 2);
}
