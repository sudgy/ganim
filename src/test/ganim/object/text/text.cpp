#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/ga_equals.hpp"

#include <iostream>

#include "ganim/object/text/text.hpp"
#include "ganim/object/text/text_helpers.hpp"

using namespace ganim;

TEST_CASE("Text", "[object][text]") {
    auto do_test = []{
        auto text1 = make_text("Wo", "rld");
        REQUIRE(text1->size() == 2);
        text1[1]->set_color("FF0000");
        auto text2 = make_text("W", "o", "r", "l", "d");
        REQUIRE(text2->size() == 5);
        text2[2]->set_color("FF0000");
        text2[3]->set_color("FF0000");
        text2[4]->set_color("FF0000");

        auto logical1 = text1[0]->get_logical_bounding_box();
        auto logical2 = text1[1]->get_logical_bounding_box();
        auto true1 = text1[0]->get_true_bounding_box();
        auto true2 = text1[1]->get_true_bounding_box();

        using namespace vga2;
        REQUIRE_THAT(logical1.get_lower_left<VGA2>(),
                GAEquals(-1.32306*e1 - 0.25*e2, 1e-5));
        REQUIRE_THAT(logical1.get_upper_right<VGA2>(),
                GAEquals(0.0984497*e1 + 0.75*e2, 1e-5));
        REQUIRE_THAT(logical2.get_lower_left<VGA2>(),
                GAEquals(0.1297*e1 - 0.25*e2, 1e-5));
        REQUIRE_THAT(logical2.get_upper_right<VGA2>(),
                GAEquals(1.32306*e1 + 0.75*e2, 1e-5));
        REQUIRE_THAT(true1.get_lower_left<VGA2>(),
                GAEquals(-1.32306*e1 - 0.03125*e2, 1e-5));
        REQUIRE_THAT(true1.get_upper_right<VGA2>(),
                GAEquals(0.0984497*e1 + 0.6875*e2, 1e-5));
        REQUIRE_THAT(true2.get_lower_left<VGA2>(),
                GAEquals(0.1297*e1 - 0.015625*e2, 1e-5));
        REQUIRE_THAT(true2.get_upper_right<VGA2>(),
                GAEquals(1.32306*e1 + 0.703125*e2, 1e-5));

        auto scene = TestScene(16, 16, 4, 4, 1);
        scene.check_draw_equivalent(text1, text2);

        scene.add(text1);
        text1->set_visible(true);
        scene.frame_advance();
        auto found_white = false;
        auto found_red = false;
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                auto pixel = scene.get_pixel(0, x, y);
                if (pixel.r > 0) {
                    if (pixel.g == pixel.r and pixel.b == pixel.r) {
                        found_white = true;
                    }
                    if (pixel.g == 0 and pixel.b == 0) {
                        found_red = true;
                    }
                }
            }
        }
        REQUIRE(found_white);
        REQUIRE(found_red);
    };
    do_test();
    set_text_texture_size(1024);
    INFO("Second time");
    do_test();
}

TEST_CASE("Text newlines", "[object][text]") {
    auto test1 = make_text({0.5}, "Hello\nworld");
    auto test2 = make_text({1.5}, "Hello\n", "world");
    auto test3 = make_text({2.5}, "A very long ", "hello\n world");
    auto box1 = test1->get_logical_bounding_box();
    auto box21 = test2[0]->get_logical_bounding_box();
    auto box22 = test2[1]->get_logical_bounding_box();
    auto box31 = test3[0]->get_logical_bounding_box();
    auto box32 = test3[1]->get_logical_bounding_box();

    using namespace vga2;

    REQUIRE(box1.get_x() == 0);
    REQUIRE(box1.get_height() == 2.5);

    REQUIRE(box21.get_x() == 0);
    REQUIRE(box22.get_x() == 0);
    REQUIRE(box21.get_y() - 2.5 == box22.get_y());

    REQUIRE(box31.get_x() < 0);
    REQUIRE(box32.get_x() > 0);
    REQUIRE(box31.get_y() - 1.75 == box32.get_y());
}
