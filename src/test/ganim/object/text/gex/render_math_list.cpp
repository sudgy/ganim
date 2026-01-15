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
    REQUIRE(box.width == 1.38116319444444446);
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
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomScript(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'e')),
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'1')),
        nullptr
    )));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomScript(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'e')),
        nullptr,
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'2'))
    )));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomScript(
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

TEST_CASE("GeX render_math_list accents", "[object][text][gex]") {
    // a \mathaccent a b b
    auto list = MathList();
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSymbol(U'a')));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomAccent(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'a')),
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'b'))
    )));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSymbol(U'b')));
    auto box = render_math_list(list, Style::Display);

    REQUIRE(box.glyphs.size() == 4);
}

TEST_CASE("GeX render_math_list fractions", "[object][text][gex]") {
    // a {a \over a} {a \abovewithdelims() 10pt a} =
    auto list = MathList();
    auto a = Atom(Box(), AtomType::Ord, AtomSymbol(U'a'));
    list.emplace_back(a);
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomList({
        {FractionNoad({{a}}, {{a}}, 0, 0, -1, 0)}
    })));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomList({
        {FractionNoad({{a}}, {{a}}, U'(', U')', 1, 0)}
    })));
    list.emplace_back(Atom(Box(), AtomType::Ord, AtomSymbol(U'=')));
    auto box = render_math_list(list, Style::Display);

    REQUIRE(box.glyphs.size() == 10);
    REQUIRE(box.glyphs[0].texture_x == box.glyphs[1].texture_x);
    REQUIRE(box.glyphs[0].texture_x == box.glyphs[3].texture_x);
    REQUIRE(box.glyphs[0].texture_x == box.glyphs[5].texture_x);
    REQUIRE(box.glyphs[0].texture_x == box.glyphs[7].texture_x);

    REQUIRE(box.glyphs[2].texture_x == 0);
    REQUIRE(box.glyphs[2].texture_y == 0);
    REQUIRE(box.glyphs[6].texture_x == 0);
    REQUIRE(box.glyphs[6].texture_y == 0);
    REQUIRE(box.glyphs[2].height == 0.04);
    REQUIRE(box.glyphs[6].height == 1);
    REQUIRE(box.glyphs[2].width >= box.glyphs[1].width);
    REQUIRE(box.glyphs[2].width >= box.glyphs[3].width);
    REQUIRE(box.glyphs[6].width >= box.glyphs[5].width);
    REQUIRE(box.glyphs[6].width >= box.glyphs[7].width);

    REQUIRE(box.glyphs[0].x_pos < box.glyphs[1].x_pos);
    REQUIRE(box.glyphs[0].x_pos < box.glyphs[2].x_pos);
    REQUIRE(box.glyphs[0].x_pos < box.glyphs[3].x_pos);
    REQUIRE(box.glyphs[1].x_pos < box.glyphs[4].x_pos);
    REQUIRE(box.glyphs[2].x_pos < box.glyphs[4].x_pos);
    REQUIRE(box.glyphs[3].x_pos < box.glyphs[4].x_pos);
    REQUIRE(box.glyphs[4].x_pos < box.glyphs[5].x_pos);
    REQUIRE(box.glyphs[4].x_pos < box.glyphs[6].x_pos);
    REQUIRE(box.glyphs[4].x_pos < box.glyphs[7].x_pos);
    REQUIRE(box.glyphs[5].x_pos < box.glyphs[8].x_pos);
    REQUIRE(box.glyphs[6].x_pos < box.glyphs[8].x_pos);
    REQUIRE(box.glyphs[7].x_pos < box.glyphs[8].x_pos);
    REQUIRE(box.glyphs[8].x_pos < box.glyphs[9].x_pos);

    REQUIRE(box.glyphs[1].y_pos > box.glyphs[2].y_pos);
    REQUIRE(box.glyphs[2].y_pos > box.glyphs[3].y_pos);
    REQUIRE(box.glyphs[5].y_pos > box.glyphs[6].y_pos);
    REQUIRE(box.glyphs[6].y_pos > box.glyphs[7].y_pos);

    auto get_mid = [](auto& glyph) {return glyph.draw_y - glyph.height / 2;};
    REQUIRE(get_mid(box.glyphs[4]) == get_mid(box.glyphs[8]));
    REQUIRE(get_mid(box.glyphs[4]) == get_mid(box.glyphs[9]));
}

TEST_CASE("GeX render_math_list mskip", "[object][text][gex]") {
    // aa\,a\!a
    auto list = MathList();
    auto a = Atom(Box(), AtomType::Ord, AtomSymbol(U'a'));
    list.emplace_back(a);
    list.emplace_back(a);
    list.emplace_back(GlueNoad(1));
    list.emplace_back(a);
    list.emplace_back(GlueNoad(-1));
    list.emplace_back(a);
    auto box = render_math_list(list, Style::Display);

    auto dif = [&](int a, int b) {
        return box.glyphs[a].x_pos - box.glyphs[b].x_pos;
    };
    REQUIRE(box.glyphs.size() == 4);
    REQUIRE(dif(2, 1) > dif(1, 0));
    REQUIRE(dif(3, 2) < dif(1, 0));
}

TEST_CASE("GeX render_math_list radical", "[object][text][gex]") {
    // \sqrt{a}
    auto list = MathList();
    list.emplace_back(Atom(Box(), AtomType::Rad, AtomRadical(
        std::make_unique<Atom>(Box(), AtomType::Ord, AtomSymbol(U'a')),
        U'√',
        0
    )));
    auto box = render_math_list(list, Style::Display);
    REQUIRE(box.glyphs.size() == 3);
    REQUIRE(box.glyphs[0].x_pos < box.glyphs[1].x_pos);
    REQUIRE(box.glyphs[0].x_pos < box.glyphs[2].x_pos);
    REQUIRE(box.glyphs[1].y_pos > box.glyphs[2].y_pos);
}

TEST_CASE("GeX render_math_list \\text", "[object][text][gex]") {
    // a \text a
    auto list = MathList();
    list.emplace_back(Atom({}, AtomType::Ord, AtomSymbol(U'a')));
    list.emplace_back(Atom({}, AtomType::Ord, AtomTokens({
        Token(CharacterToken(U'a'), 0, 0)
    })));
    auto box = render_math_list(list, Style::Display);
    REQUIRE(box.glyphs.size() == 2);
    REQUIRE((box.glyphs[0].x_pos != box.glyphs[1].x_pos or
             box.glyphs[0].y_pos != box.glyphs[1].y_pos));
}

TEST_CASE("GeX render_math_list \\phantom", "[object][text][gex]") {
    auto a_noad = Noad(
        Atom(Box(), AtomType::Ord, AtomSymbol(U'a',0,0))
    );
    auto b_noad = Noad(
        Atom(Box(), AtomType::Ord, AtomSymbol(U'a',0,0))
    );
    auto bb_noad = Noad(Atom(
        Box(),
        AtomType::Ord,
        AtomList({b_noad, b_noad})
    ));
    // a{bb}a
    auto list1 = MathList();
    list1.emplace_back(a_noad);
    list1.emplace_back(bb_noad);
    list1.emplace_back(a_noad);
    // a\phantom{bb}a
    auto list2 = MathList();
    list2.emplace_back(a_noad);
    list2.emplace_back(CommandNoad("phantom", 0, 0));
    list2.emplace_back(bb_noad);
    list2.emplace_back(a_noad);

    auto box1 = render_math_list(list1, Style::Text);
    auto box2 = render_math_list(list2, Style::Text);
    REQUIRE(box1.glyphs.size() == 4);
    REQUIRE(box2.glyphs.size() == 4);
    REQUIRE(box2.glyphs[1].invisible);
    REQUIRE(box2.glyphs[2].invisible);
    REQUIRE(box1.glyphs[0].x_pos == box2.glyphs[0].x_pos);
    REQUIRE(box1.glyphs[0].y_pos == box2.glyphs[0].y_pos);
    REQUIRE(box1.glyphs[0].draw_x == box2.glyphs[0].draw_x);
    REQUIRE(box1.glyphs[0].draw_y == box2.glyphs[0].draw_y);
    REQUIRE(box1.glyphs[0].width == box2.glyphs[0].width);
    REQUIRE(box1.glyphs[0].height == box2.glyphs[0].height);
    REQUIRE(box1.glyphs[0].texture_x == box2.glyphs[0].texture_x);
    REQUIRE(box1.glyphs[0].texture_y == box2.glyphs[0].texture_y);
    REQUIRE(box1.glyphs[0].texture_width == box2.glyphs[0].texture_width);
    REQUIRE(box1.glyphs[0].texture_height == box2.glyphs[0].texture_height);
    REQUIRE(box1.glyphs[3].x_pos == box2.glyphs[3].x_pos);
    REQUIRE(box1.glyphs[3].y_pos == box2.glyphs[3].y_pos);
    REQUIRE(box1.glyphs[3].draw_x == box2.glyphs[3].draw_x);
    REQUIRE(box1.glyphs[3].draw_y == box2.glyphs[3].draw_y);
    REQUIRE(box1.glyphs[3].width == box2.glyphs[3].width);
    REQUIRE(box1.glyphs[3].height == box2.glyphs[3].height);
    REQUIRE(box1.glyphs[3].texture_x == box2.glyphs[3].texture_x);
    REQUIRE(box1.glyphs[3].texture_y == box2.glyphs[3].texture_y);
    REQUIRE(box1.glyphs[3].texture_width == box2.glyphs[3].texture_width);
    REQUIRE(box1.glyphs[3].texture_height == box2.glyphs[3].texture_height);
}

TEST_CASE("GeX render_math_list fraction line bug", "[object][text][gex]") {
    auto one_noad = Noad(Atom(Box(), AtomType::Ord, AtomSymbol(U'1', 0, 0)));
    auto plus_noad = Noad(Atom(Box(), AtomType::Rel, AtomSymbol(U'+', 0, 0)));
    auto num = MathList();
    auto den = MathList();
    num.emplace_back(one_noad);
    den.emplace_back(one_noad);
    den.emplace_back(plus_noad);
    den.emplace_back(one_noad);
    auto list = MathList();
    list.emplace_back(FractionNoad(num, den, 0, 0, 0.1));
    auto box = render_math_list(list, Style::Display);

    REQUIRE(box.glyphs[1].x_pos + box.glyphs[1].width > box.glyphs[4].x_pos);
}

TEST_CASE("GeX render_math_list boundaries", "[object][text][gex]") {
    // Technically not \over, it's just them on top of each other
    // \left( \left( a \right. {1 \over 2} \right)
    auto list = MathList();
    list.push_back(Noad(
        BoundaryNoad(
            {
                Noad(BoundaryNoad(
                    {
                        Noad(Atom(Box(), AtomType::Ord, AtomSymbol(U'a', 0, 0)))
                    },
                    U'(',
                    0
                )),
                Noad(FractionNoad(
                    {
                        Noad(Atom(Box(), AtomType::Ord, AtomSymbol(U'1', 0, 0)))
                    },
                    {
                        Noad(Atom(Box(), AtomType::Ord, AtomSymbol(U'2', 0, 0)))
                    }
                ))
            },
            U'(',
            U')'
        )
    ));
    auto box = render_math_list(list, Style::Display);

    REQUIRE(box.glyphs.size() == 6);
    REQUIRE(box.glyphs[0].x_pos < box.glyphs[1].x_pos);
    REQUIRE(box.glyphs[1].x_pos < box.glyphs[2].x_pos);
    REQUIRE(box.glyphs[2].x_pos < box.glyphs[3].x_pos);
    REQUIRE(box.glyphs[2].x_pos < box.glyphs[4].x_pos);
    REQUIRE(box.glyphs[3].x_pos < box.glyphs[5].x_pos);
    REQUIRE(box.glyphs[4].x_pos < box.glyphs[5].x_pos);
    REQUIRE(box.glyphs[3].y_pos > box.glyphs[4].y_pos);

    REQUIRE(box.glyphs[0].height > box.glyphs[1].height);
}

TEST_CASE("GeX render_math_list boundary atom spacing", "[object][text][gex]") {
    auto a = Noad(Atom(Box(), AtomType::Ord, AtomSymbol(U'a', 0, 0)));
    auto plus = Noad(Atom(Box(), AtomType::Bin, AtomSymbol(U'+', 0, 0)));
    auto plus_ord = Noad(Atom(Box(), AtomType::Ord, AtomSymbol(U'+', 0, 0)));
    auto boundary = Noad(BoundaryNoad({a}, U'(', U')'));
    auto list1 = MathList{
        boundary,
        plus,
        a
    };
    auto list2 = MathList{
        boundary,
        plus_ord,
        a
    };
    auto box1 = render_math_list(list1, Style::Display);
    auto box2 = render_math_list(list2, Style::Display);
    REQUIRE(box1.glyphs[3].x_pos > box2.glyphs[3].x_pos);
}
