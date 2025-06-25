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
                AtomSymbol(codepoint, group, 0)
            )
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
    REQUIRE(box1.width == 3.87847222222222232);
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
    REQUIRE(box1.glyphs[1].x_pos == 0.75434027777777779);
    REQUIRE(box1.glyphs[1].y_pos == 0);
    REQUIRE(box1.glyphs[1].draw_x == 0.80121527777777779);
    REQUIRE(box1.glyphs[1].draw_y == 0.6015625);
    REQUIRE(box1.glyphs[1].width == 0.6875);
    REQUIRE(box1.glyphs[1].height == 0.6953125);
    REQUIRE(box1.glyphs[1].y_min == -0.2578125);
    REQUIRE(box1.glyphs[1].y_max == 0.75);
    REQUIRE(box1.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box1.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box1.glyphs[1].group_index == 1);
    REQUIRE(box1.glyphs[2].x_pos == 1.71961805555555536);
    REQUIRE(box1.glyphs[2].y_pos == 0);
    REQUIRE(box1.glyphs[2].draw_x == 1.75868055555555536);
    REQUIRE(box1.glyphs[2].draw_y == 0.703125);
    REQUIRE(box1.glyphs[2].width == 0.390625);
    REQUIRE(box1.glyphs[2].height == 0.71875);
    REQUIRE(box1.glyphs[2].y_min == -0.2578125);
    REQUIRE(box1.glyphs[2].y_max == 0.75);
    REQUIRE(box1.glyphs[2].texture_width == 0.048828125f);
    REQUIRE(box1.glyphs[2].texture_height == 0.08984375f);
    REQUIRE(box1.glyphs[2].group_index == 2);
    REQUIRE(box1.glyphs[3].x_pos == 2.44357638888888884);
    REQUIRE(box1.glyphs[3].y_pos == 0);
    REQUIRE(box1.glyphs[3].draw_x == 2.49045138888888884);
    REQUIRE(box1.glyphs[3].draw_y == 0.375);
    REQUIRE(box1.glyphs[3].width == 0.6875);
    REQUIRE(box1.glyphs[3].height == 0.25);
    REQUIRE(box1.glyphs[3].y_min == -0.2578125);
    REQUIRE(box1.glyphs[3].y_max == 0.75);
    REQUIRE(box1.glyphs[3].texture_width == 0.0859375f);
    REQUIRE(box1.glyphs[3].texture_height == 0.03125f);
    REQUIRE(box1.glyphs[3].group_index == 3);
    REQUIRE(box1.glyphs[4].x_pos == 3.46440972222222232);
    REQUIRE(box1.glyphs[4].y_pos == 0);
    REQUIRE(box1.glyphs[4].draw_x == 3.49565972222222232);
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
    REQUIRE(box3.width == 2.88819715711805536);
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
    REQUIRE(box3.glyphs[1].x_pos == 0.75434027777777779);
    REQUIRE(box3.glyphs[1].y_pos == 0);
    REQUIRE(box3.glyphs[1].draw_x == 0.80121527777777779);
    REQUIRE(box3.glyphs[1].draw_y == 0.6015625);
    REQUIRE(box3.glyphs[1].width == 0.6875);
    REQUIRE(box3.glyphs[1].height == 0.6953125);
    REQUIRE(box3.glyphs[1].y_min == -0.2578125);
    REQUIRE(box3.glyphs[1].y_max == 0.75);
    REQUIRE(box3.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box3.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box3.glyphs[1].group_index == 1);
    REQUIRE(box3.glyphs[2].x_pos == 1.71961805555555536);
    REQUIRE(box3.glyphs[2].y_pos == 0);
    REQUIRE(box3.glyphs[2].draw_x == 1.76649305555555536);
    REQUIRE(box3.glyphs[2].draw_y == 0.6015625);
    REQUIRE(box3.glyphs[2].width == 0.6875);
    REQUIRE(box3.glyphs[2].height == 0.6953125);
    REQUIRE(box3.glyphs[2].y_min == -0.2578125);
    REQUIRE(box3.glyphs[2].y_max == 0.75);
    REQUIRE(box3.glyphs[2].texture_width == 0.0859375f);
    REQUIRE(box3.glyphs[2].texture_height == 0.086914062f);
    REQUIRE(box3.glyphs[2].group_index == 2);
    REQUIRE(box3.glyphs[3].x_pos == 2.49757215711805536);
    REQUIRE(box3.glyphs[3].y_pos == 0);
    REQUIRE(box3.glyphs[3].draw_x == 2.53663465711805536);
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
                AtomSymbol(codepoint, group, 0)
            )
        );
    };
    using enum AtomType;
    add_to_list(list, Ord, U'a', 0);
    list.emplace_back(CommandNoad("scriptstyle", 1, 0));
    add_to_list(list, Ord, U'+', 1);
    list.emplace_back(CommandNoad("displaystyle", 2, 0));
    add_to_list(list, Ord, U'a', 2);
    auto box = render_math_list(list, Style::Text);

    REQUIRE(box.height == 0.453125);
    REQUIRE(box.depth == 0.06562499999999999);
    REQUIRE(box.width == 1.52881944444444429);
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
    REQUIRE(box.glyphs[1].x_pos == 0.51545138888888886);
    REQUIRE(box.glyphs[1].y_pos == 0);
    REQUIRE(box.glyphs[1].draw_x == 0.54826388888888888);
    REQUIRE(box.glyphs[1].draw_y == 0.42109374999999999);
    REQUIRE(box.glyphs[1].width == 0.48124999999999996);
    REQUIRE(box.glyphs[1].height == 0.48671874999999998);
    REQUIRE(box.glyphs[1].y_min == -0.18046874999999998);
    REQUIRE(box.glyphs[1].y_max == 0.52499999999999991);
    REQUIRE(box.glyphs[1].texture_width == 0.0859375f);
    REQUIRE(box.glyphs[1].texture_height == 0.086914062f);
    REQUIRE(box.glyphs[1].group_index == 1);
    REQUIRE(box.glyphs[2].x_pos == 1.05225694444444429);
    REQUIRE(box.glyphs[2].y_pos == 0);
    REQUIRE(box.glyphs[2].draw_x == 1.08350694444444429);
    REQUIRE(box.glyphs[2].draw_y == 0.453125);
    REQUIRE(box.glyphs[2].width == 0.4765625);
    REQUIRE(box.glyphs[2].height == 0.46875);
    REQUIRE(box.glyphs[2].y_min == -0.2578125);
    REQUIRE(box.glyphs[2].y_max == 0.75);
    REQUIRE(box.glyphs[2].texture_width == 0.059570312f);
    REQUIRE(box.glyphs[2].texture_height == 0.05859375f);
    REQUIRE(box.glyphs[2].group_index == 2);
}

TEST_CASE("GeX render_math_list groups/styles", "[object][text][gex]") {
    // a {\scriptstyle a {a \textstyle a} a} a
    auto list = MathList();
    auto a_noad = Noad(
        Atom(Box(), AtomType::Ord, AtomSymbol(U'a',0,0))
    );
    list.emplace_back(a_noad);
    list.emplace_back(
        Atom(Box(), AtomType::Ord, AtomList({
            Noad(CommandNoad("scriptstyle", 0, 0)),
            a_noad,
            Noad(Atom(Box(), AtomType::Ord, AtomList({
                a_noad,
                Noad(CommandNoad("textstyle", 0, 0)),
                a_noad
            }))),
            a_noad
        }))
    );
    list.emplace_back(a_noad);
    auto box = render_math_list(list, Style::Text);

    REQUIRE(box.glyphs.size() == 6);
    REQUIRE(box.glyphs[0].width >  box.glyphs[1].width);
    REQUIRE(box.glyphs[2].width == box.glyphs[1].width);
    REQUIRE(box.glyphs[3].width == box.glyphs[0].width);
    REQUIRE(box.glyphs[4].width == box.glyphs[1].width);
    REQUIRE(box.glyphs[5].width == box.glyphs[0].width);
}
