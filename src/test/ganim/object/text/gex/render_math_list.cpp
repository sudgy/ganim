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
    REQUIRE(box1.width == 3.98958333333333348);
    REQUIRE(box1.glyphs.size() == 5);
    REQUIRE(box1.glyphs[0].x_pos == 0);
    REQUIRE(box1.glyphs[0].y_pos == 0);
    REQUIRE(box1.glyphs[0].group_index == 0);
    REQUIRE(box1.glyphs[1].x_pos > box1.glyphs[0].x_pos);
    REQUIRE(box1.glyphs[1].y_pos == 0);
    REQUIRE(box1.glyphs[1].draw_x > box1.glyphs[0].draw_x);
    REQUIRE(box1.glyphs[1].group_index == 1);
    REQUIRE(box1.glyphs[2].x_pos > box1.glyphs[1].x_pos);
    REQUIRE(box1.glyphs[2].y_pos == 0);
    REQUIRE(box1.glyphs[2].draw_x > box1.glyphs[1].draw_x);
    REQUIRE(box1.glyphs[2].group_index == 2);
    REQUIRE(box1.glyphs[3].x_pos > box1.glyphs[2].x_pos);
    REQUIRE(box1.glyphs[3].y_pos == 0);
    REQUIRE(box1.glyphs[3].draw_x > box1.glyphs[2].draw_x);
    REQUIRE(box1.glyphs[3].group_index == 3);
    REQUIRE(box1.glyphs[4].x_pos > box1.glyphs[3].x_pos);
    REQUIRE(box1.glyphs[4].y_pos == 0);
    REQUIRE(box1.glyphs[4].draw_x > box1.glyphs[3].draw_x);
    REQUIRE(box1.glyphs[4].group_index == 4);

    REQUIRE(box2.height == 0.6015625);
    REQUIRE(box2.depth == 0.09375);
    REQUIRE(box2.width == 1.9945068359375);
    REQUIRE(box2.glyphs.size() == 3);
    REQUIRE(box2.glyphs[0].x_pos == 0);
    REQUIRE(box2.glyphs[0].y_pos == 0);
    REQUIRE(box2.glyphs[0].group_index == 0);
    REQUIRE(box2.glyphs[1].x_pos > box1.glyphs[0].x_pos);
    REQUIRE(box2.glyphs[1].y_pos == 0);
    REQUIRE(box2.glyphs[1].draw_x > box1.glyphs[0].draw_x);
    REQUIRE(box2.glyphs[1].group_index == 1);
    REQUIRE(box2.glyphs[2].x_pos > box1.glyphs[1].x_pos);
    REQUIRE(box2.glyphs[2].y_pos == 0);
    REQUIRE(box2.glyphs[2].draw_x > box1.glyphs[1].draw_x);
    REQUIRE(box2.glyphs[2].group_index == 2);

    REQUIRE(box3.height == 0.703125);
    REQUIRE(box3.depth == 0.09375);
    REQUIRE(box3.width == 2.94375271267361116);
    REQUIRE(box3.glyphs.size() == 4);
    REQUIRE(box3.glyphs[0].x_pos == 0);
    REQUIRE(box3.glyphs[0].y_pos == 0);
    REQUIRE(box3.glyphs[0].group_index == 0);
    REQUIRE(box3.glyphs[1].x_pos > box1.glyphs[0].x_pos);
    REQUIRE(box3.glyphs[1].y_pos == 0);
    REQUIRE(box3.glyphs[1].draw_x > box1.glyphs[0].draw_x);
    REQUIRE(box3.glyphs[1].group_index == 1);
    REQUIRE(box3.glyphs[2].x_pos > box1.glyphs[1].x_pos);
    REQUIRE(box3.glyphs[2].y_pos == 0);
    REQUIRE(box3.glyphs[2].draw_x > box1.glyphs[1].draw_x);
    REQUIRE(box3.glyphs[2].group_index == 2);
    REQUIRE(box3.glyphs[3].x_pos > box1.glyphs[2].x_pos);
    REQUIRE(box3.glyphs[3].y_pos == 0);
    REQUIRE(box3.glyphs[3].draw_x > box1.glyphs[2].draw_x);
    REQUIRE(box3.glyphs[3].group_index == 3);

    REQUIRE(box4.height == 0.703125);
    REQUIRE(box4.depth == 0.09375);
    REQUIRE(box4.width == 3.79485405815972232);
    REQUIRE(box4.glyphs.size() == 5);
    REQUIRE(box4.glyphs[0].x_pos == 0);
    REQUIRE(box4.glyphs[0].y_pos == 0);
    REQUIRE(box4.glyphs[0].group_index == 0);
    REQUIRE(box4.glyphs[1].x_pos > box1.glyphs[0].x_pos);
    REQUIRE(box4.glyphs[1].y_pos == 0);
    REQUIRE(box4.glyphs[1].draw_x > box1.glyphs[0].draw_x);
    REQUIRE(box4.glyphs[1].group_index == 1);
    REQUIRE(box4.glyphs[2].x_pos > box1.glyphs[1].x_pos);
    REQUIRE(box4.glyphs[2].y_pos == 0);
    REQUIRE(box4.glyphs[2].draw_x > box1.glyphs[1].draw_x);
    REQUIRE(box4.glyphs[2].group_index == 2);
    REQUIRE(box4.glyphs[3].x_pos > box1.glyphs[2].x_pos);
    REQUIRE(box4.glyphs[3].y_pos == 0);
    REQUIRE(box4.glyphs[3].draw_x > box1.glyphs[2].draw_x);
    REQUIRE(box4.glyphs[3].group_index == 3);
    REQUIRE(box4.glyphs[4].x_pos > box1.glyphs[3].x_pos);
    REQUIRE(box4.glyphs[4].y_pos == 0);
    REQUIRE(box4.glyphs[4].draw_x > box1.glyphs[3].draw_x);
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
    add_to_list(list, Ord, U'a', 1);
    list.emplace_back(CommandNoad("displaystyle", 2, 0));
    add_to_list(list, Ord, U'a', 2);
    auto box = render_math_list(list, Style::Text);

    REQUIRE(box.height == 0.453125);
    REQUIRE(box.depth == 0.015625);
    REQUIRE(box.width == 1.33394097222222219);
    REQUIRE(box.glyphs.size() == 3);
    REQUIRE(box.glyphs[0].x_pos == 0);
    REQUIRE(box.glyphs[0].y_pos == 0);
    REQUIRE(box.glyphs[0].group_index == 0);
    REQUIRE(box.glyphs[1].x_pos > box.glyphs[0].x_pos);
    REQUIRE(box.glyphs[1].y_pos == 0);
    REQUIRE(box.glyphs[1].width < box.glyphs[0].width);
    REQUIRE(box.glyphs[1].height < box.glyphs[0].height);
    REQUIRE(box.glyphs[1].y_min > box.glyphs[0].y_min);
    REQUIRE(box.glyphs[1].y_max < box.glyphs[0].y_max);
    REQUIRE(box.glyphs[1].group_index == 1);
    REQUIRE(box.glyphs[2].x_pos > box.glyphs[1].x_pos);
    REQUIRE(box.glyphs[2].y_pos == 0);
    REQUIRE(box.glyphs[2].width == box.glyphs[0].width);
    REQUIRE(box.glyphs[2].height == box.glyphs[0].height);
    REQUIRE(box.glyphs[2].y_min == box.glyphs[0].y_min);
    REQUIRE(box.glyphs[2].y_max == box.glyphs[0].y_max);
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

TEST_CASE("GeX render_math_list sub/superscripts", "[object][text][gex]") {
    // 1 2 e e_1 e^2 e_1^2
    auto list = MathList();
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSymbol(U'1')));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSymbol(U'2')));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSymbol(U'e')));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSubscript(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'e')),
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'1'))
    )));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSuperscript(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'e')),
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'2'))
    )));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSubsuperscript(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'e')),
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'1')),
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'2'))
    )));
    auto box = render_math_list(list, Style::Display);
    auto check_later = [&](int i, int j) {
        INFO("Comparing " << i << " and " << j);
        auto& glyph1 = box.glyphs[i];
        auto& glyph2 = box.glyphs[j];
        REQUIRE(glyph2.x_pos > glyph1.x_pos);
        REQUIRE(glyph2.draw_x > glyph1.x_pos);
        REQUIRE(glyph2.texture_x == glyph1.texture_x);
        REQUIRE(glyph2.texture_y == glyph1.texture_y);
        REQUIRE(glyph2.texture_width == glyph1.texture_width);
        REQUIRE(glyph2.texture_height == glyph1.texture_height);
    };
    auto check_later_sub = [&](int i, int j) {
        check_later(i, j);
        INFO("Comparing " << i << " and " << j);
        auto& glyph1 = box.glyphs[i];
        auto& glyph2 = box.glyphs[j];
        REQUIRE(glyph2.y_pos < glyph1.y_pos);
        REQUIRE(glyph2.draw_y < glyph1.draw_y);
        REQUIRE(glyph2.width < glyph1.width);
        REQUIRE(glyph2.height < glyph1.height);
        REQUIRE(glyph2.y_min < glyph1.y_min);
        REQUIRE(glyph2.y_max < glyph1.y_max);
    };
    auto check_later_sup = [&](int i, int j) {
        check_later(i, j);
        INFO("Comparing " << i << " and " << j);
        auto& glyph1 = box.glyphs[i];
        auto& glyph2 = box.glyphs[j];
        REQUIRE(glyph2.y_pos > glyph1.y_pos);
        REQUIRE(glyph2.draw_y > glyph1.draw_y);
        REQUIRE(glyph2.width < glyph1.width);
        REQUIRE(glyph2.height < glyph1.height);
        REQUIRE(glyph2.y_min > glyph1.y_min);
        REQUIRE(glyph2.y_max > glyph1.y_max);
    };
    auto check_later_same = [&](int i, int j) {
        check_later(i, j);
        INFO("Comparing " << i << " and " << j);
        auto& glyph1 = box.glyphs[i];
        auto& glyph2 = box.glyphs[j];
        REQUIRE(glyph2.y_pos == glyph1.y_pos);
        REQUIRE(glyph2.draw_y == glyph1.draw_y);
        REQUIRE(glyph2.width == glyph1.width);
        REQUIRE(glyph2.height == glyph1.height);
        REQUIRE(glyph2.y_min == glyph1.y_min);
        REQUIRE(glyph2.y_max == glyph1.y_max);
    };
    REQUIRE(box.glyphs.size() == 10);
    check_later_same(2, 3);
    check_later_same(2, 5);
    check_later_same(2, 7);
    check_later_sub(0, 4);
    // 8 and 9 are swapped because of drawing superscripts before subscripts
    check_later_sub(0, 9);
    check_later_sup(1, 6);
    check_later_sup(1, 8);
    check_later_same(6, 8);
    REQUIRE(box.glyphs[9].y_pos < box.glyphs[4].y_pos);
}
