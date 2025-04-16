#include <catch2/catch_test_macros.hpp>

#include "ganim/util/box.hpp"

#include "test/ganim/ga_equals.hpp"

#include "ganim/ga/exp.hpp"
#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("merge_boxes", "[object]") {
    using namespace vga3;
    const auto box1 = Box(Vec{0, 0, 0}, Vec{1, 1, 1});
    const auto box2 = Box(Vec{0, 0, 0}, Vec{3, 3, 3});
    const auto box3 = Box(Vec{1, 1, 1}, Vec{2, 2, 2});
    const auto box4 = Box(Vec{4, 5, 6}, Vec{5, 6, 7});
    const auto box5 = Box(Vec{1, 2, 3}, Vec{3, 4, 5});
    const auto box6 = Box(Vec{1, 1, -1}, Vec{2, 2, 1});

    const auto res1 = merge_boxes(box1, box2);
    const auto res2 = merge_boxes(box2, box3);
    const auto res3 = merge_boxes(box1, box4);
    const auto res4 = merge_boxes(box2, box5);
    const auto res5 = merge_boxes(box2, box6);
    const auto res1p1 = pga3_to_vga3(res1.get_inner_lower_left());
    const auto res1p2 = pga3_to_vga3(res1.get_outer_upper_right());
    const auto res2p1 = pga3_to_vga3(res2.get_inner_lower_left());
    const auto res2p2 = pga3_to_vga3(res2.get_outer_upper_right());
    const auto res3p1 = pga3_to_vga3(res3.get_inner_lower_left());
    const auto res3p2 = pga3_to_vga3(res3.get_outer_upper_right());
    const auto res4p1 = pga3_to_vga3(res4.get_inner_lower_left());
    const auto res4p2 = pga3_to_vga3(res4.get_outer_upper_right());
    const auto res5p1 = pga3_to_vga3(res5.get_inner_lower_left());
    const auto res5p2 = pga3_to_vga3(res5.get_outer_upper_right());

    REQUIRE_THAT(res1p1, GAEquals(0));
    REQUIRE_THAT(res1p2, GAEquals(3*e1 + 3*e2 + 3*e3));
    REQUIRE_THAT(res2p1, GAEquals(0));
    REQUIRE_THAT(res2p2, GAEquals(3*e1 + 3*e2 + 3*e3));
    REQUIRE_THAT(res3p1, GAEquals(0));
    REQUIRE_THAT(res3p2, GAEquals(5*e1 + 6*e2 + 7*e3));
    REQUIRE_THAT(res4p1, GAEquals(0));
    REQUIRE_THAT(res4p2, GAEquals(3*e1 + 4*e2 + 5*e3));
    REQUIRE_THAT(res5p1, GAEquals(-e3));
    REQUIRE_THAT(res5p2, GAEquals(3*e1 + 3*e2 + 3*e3));
}

TEST_CASE("transform_box", "[object]") {
    using namespace vga3;
    const auto box1 = Box(Vec{-1, -1, 0}, Vec{1, 1, 0});
    const auto box2 = Box(Vec{-1, -1, -1}, Vec{1, 1, 1});

    auto res1 = transform_box(box1, 1);
    auto res2 = transform_box(box1, 1 + pga3::e01);
    auto res3 = transform_box(box2, 1 + pga3::e01);
    auto res4 = transform_box(box1, ga_exp(pga3::e12*τ/16));
    auto res5 = transform_box(box2, ga_exp(pga3::e12*τ/16));
    auto res6 = transform_box(box1, ga_exp(pga3::e13*τ/16));
    auto res7 = transform_box(box2, ga_exp(pga3::e13*τ/16));
    auto res8 = transform_box(box1, 1, 2);
    auto res9 = transform_box(box1, 1 + pga3::e01, 2);
    const auto res1p1 = pga3_to_vga3(res1.get_inner_lower_left());
    const auto res1p2 = pga3_to_vga3(res1.get_outer_upper_right());
    const auto res2p1 = pga3_to_vga3(res2.get_inner_lower_left());
    const auto res2p2 = pga3_to_vga3(res2.get_outer_upper_right());
    const auto res3p1 = pga3_to_vga3(res3.get_inner_lower_left());
    const auto res3p2 = pga3_to_vga3(res3.get_outer_upper_right());
    const auto res4p1 = pga3_to_vga3(res4.get_inner_lower_left());
    const auto res4p2 = pga3_to_vga3(res4.get_outer_upper_right());
    const auto res5p1 = pga3_to_vga3(res5.get_inner_lower_left());
    const auto res5p2 = pga3_to_vga3(res5.get_outer_upper_right());
    const auto res6p1 = pga3_to_vga3(res6.get_inner_lower_left());
    const auto res6p2 = pga3_to_vga3(res6.get_outer_upper_right());
    const auto res7p1 = pga3_to_vga3(res7.get_inner_lower_left());
    const auto res7p2 = pga3_to_vga3(res7.get_outer_upper_right());
    const auto res8p1 = pga3_to_vga3(res8.get_inner_lower_left());
    const auto res8p2 = pga3_to_vga3(res8.get_outer_upper_right());
    const auto res9p1 = pga3_to_vga3(res9.get_inner_lower_left());
    const auto res9p2 = pga3_to_vga3(res9.get_outer_upper_right());

    using namespace vga3;

    REQUIRE_THAT(res1p1, GAEquals(  -e1 -   e2));
    REQUIRE_THAT(res1p2, GAEquals(   e1 +   e2));
    REQUIRE_THAT(res2p1, GAEquals(   e1 -   e2));
    REQUIRE_THAT(res2p2, GAEquals( 3*e1 +   e2));
    REQUIRE_THAT(res3p1, GAEquals(   e1 -   e2 - e3));
    REQUIRE_THAT(res3p2, GAEquals( 3*e1 +   e2 + e3));
    REQUIRE_THAT(res4p1, GAEquals( (-e1 -   e2)*std::sqrt(2), 1e-5));
    REQUIRE_THAT(res4p2, GAEquals( (+e1 +   e2)*std::sqrt(2), 1e-5));
    REQUIRE_THAT(res5p1, GAEquals( (-e1 -   e2)*std::sqrt(2) - e3, 1e-5));
    REQUIRE_THAT(res5p2, GAEquals( (+e1 +   e2)*std::sqrt(2) + e3, 1e-5));
    REQUIRE_THAT(res6p1, GAEquals( (-e1 -   e3)/std::sqrt(2) - e2, 1e-5));
    REQUIRE_THAT(res6p2, GAEquals( ( e1 +   e3)/std::sqrt(2) + e2, 1e-5));
    REQUIRE_THAT(res7p1, GAEquals( (-e1 -   e3)*std::sqrt(2) - e2, 1e-5));
    REQUIRE_THAT(res7p2, GAEquals( ( e1 +   e3)*std::sqrt(2) + e2, 1e-5));
    REQUIRE_THAT(res8p1, GAEquals(-2*e1 - 2*e2));
    REQUIRE_THAT(res8p2, GAEquals( 2*e1 + 2*e2));
    REQUIRE_THAT(res9p1, GAEquals(      - 2*e2));
    REQUIRE_THAT(res9p2, GAEquals( 4*e1 + 2*e2));
}

TEST_CASE("Box sides", "[object]") {
    const auto box = Box(vga3::Vec{1, 2, 3}, vga3::Vec{4, 5, 6});

    {
        using namespace pga3;
        REQUIRE_THAT(box.get_center<PGA3>(),
                GAEquals(!(e0 + 2.5*e1 + 3.5*e2 + 4.5*e3)));
        REQUIRE_THAT(box.get_left<PGA3>(),
                GAEquals(!(e0 + e1 + 3.5*e2 + 4.5*e3)));
        REQUIRE_THAT(box.get_right<PGA3>(),
                GAEquals(!(e0 + 4*e1 + 3.5*e2 + 4.5*e3)));
        REQUIRE_THAT(box.get_up<PGA3>(),
                GAEquals(!(e0 + 2.5*e1 + 5*e2 + 4.5*e3)));
        REQUIRE_THAT(box.get_down<PGA3>(),
                GAEquals(!(e0 + 2.5*e1 + 2*e2 + 4.5*e3)));
        REQUIRE_THAT(box.get_out<PGA3>(),
                GAEquals(!(e0 + 2.5*e1 + 3.5*e2 + 6*e3)));
        REQUIRE_THAT(box.get_in<PGA3>(),
                GAEquals(!(e0 + 2.5*e1 + 3.5*e2 + 3*e3)));
        REQUIRE_THAT(box.get_outer_upper_left(),
                GAEquals(!(e0 + 1*e1 + 5*e2 + 6*e3)));
        REQUIRE_THAT(box.get_outer_upper_right(),
                GAEquals(!(e0 + 4*e1 + 5*e2 + 6*e3)));
        REQUIRE_THAT(box.get_outer_lower_left(),
                GAEquals(!(e0 + 1*e1 + 2*e2 + 6*e3)));
        REQUIRE_THAT(box.get_outer_lower_right(),
                GAEquals(!(e0 + 4*e1 + 2*e2 + 6*e3)));
        REQUIRE_THAT(box.get_inner_upper_left(),
                GAEquals(!(e0 + 1*e1 + 5*e2 + 3*e3)));
        REQUIRE_THAT(box.get_inner_upper_right(),
                GAEquals(!(e0 + 4*e1 + 5*e2 + 3*e3)));
        REQUIRE_THAT(box.get_inner_lower_left(),
                GAEquals(!(e0 + 1*e1 + 2*e2 + 3*e3)));
        REQUIRE_THAT(box.get_inner_lower_right(),
                GAEquals(!(e0 + 4*e1 + 2*e2 + 3*e3)));
    }
    {
        const auto center = box.get_center();
        const auto left = box.get_left();
        const auto right = box.get_right();
        const auto up = box.get_up();
        const auto down = box.get_down();
        const auto ur = box.get_upper_right();
        const auto ul = box.get_upper_left();
        const auto lr = box.get_lower_right();
        const auto ll = box.get_lower_left();

        using namespace pga2;
        REQUIRE_THAT(center, GAEquals(!(e0 + 2.5*e1 + 3.5*e2)));
        REQUIRE_THAT(left, GAEquals(!(e0 + e1 + 3.5*e2)));
        REQUIRE_THAT(right, GAEquals(!(e0 + 4*e1 + 3.5*e2)));
        REQUIRE_THAT(up, GAEquals(!(e0 + 2.5*e1 + 5*e2)));
        REQUIRE_THAT(down, GAEquals(!(e0 + 2.5*e1 + 2*e2)));
        REQUIRE_THAT(ur, GAEquals(!(e0 + 4*e1 + 5*e2)));
        REQUIRE_THAT(ul, GAEquals(!(e0 + 1*e1 + 5*e2)));
        REQUIRE_THAT(lr, GAEquals(!(e0 + 4*e1 + 2*e2)));
        REQUIRE_THAT(ll, GAEquals(!(e0 + 1*e1 + 2*e2)));
    }
}

TEST_CASE("Box get_outside_point", "[object]") {
    const auto box1 = Box(vga3::Vec{0, 0, 0}, vga3::Vec{1, 1, 1});
    const auto box2 = Box(vga3::Vec{-1, -1, -1}, vga3::Vec{1, 3, 3});

    {
        using namespace pga2;
        REQUIRE_THAT(box1.get_outside_point(e1).undual(),
                GAEquals(e0 + e1 + 0.5*e2));
        REQUIRE_THAT(box1.get_outside_point(-e1).undual(),
                GAEquals(e0 + 0.5*e2));
        REQUIRE_THAT(box1.get_outside_point(e2).undual(),
                GAEquals(e0 + 0.5*e1 + e2));
        REQUIRE_THAT(box1.get_outside_point(-e2).undual(),
                GAEquals(e0 + 0.5*e1));
        REQUIRE_THAT(box1.get_outside_point(e1 + e2).undual(),
                GAEquals(e0 + e1 + e2));
        REQUIRE_THAT(box1.get_outside_point(2*e1 + e2).undual(),
                GAEquals(e0 + e1 + 0.75*e2));

        REQUIRE_THAT(box2.get_outside_point(e1).undual(),
                GAEquals(e0 + e1 + e2));
        REQUIRE_THAT(box2.get_outside_point(-e1).undual(),
                GAEquals(e0 - e1 + e2));
        REQUIRE_THAT(box2.get_outside_point(e2).undual(),
                GAEquals(e0 + 3*e2));
        REQUIRE_THAT(box2.get_outside_point(-e2).undual(),
                GAEquals(e0 - e2));
        REQUIRE_THAT(box2.get_outside_point(e1 + e2).undual(),
                GAEquals(e0 + e1 + 2*e2));
        REQUIRE_THAT(box2.get_outside_point(2*e1 + e2).undual(),
                GAEquals(e0 + e1 + 1.5*e2));
    }
    {
        using namespace pga3;
        REQUIRE_THAT(box1.get_outside_point<PGA3>(e1).undual(),
                GAEquals(e0 + e1 + 0.5*e2 + 0.5*e3));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(-e1).undual(),
                GAEquals(e0 + 0.5*e2 + 0.5*e3));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(e2).undual(),
                GAEquals(e0 + 0.5*e1 + e2 + 0.5*e3));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(-e2).undual(),
                GAEquals(e0 + 0.5*e1 + 0.5*e3));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(e3).undual(),
                GAEquals(e0 + 0.5*e1 + 0.5*e2 + e3));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(-e3).undual(),
                GAEquals(e0 + 0.5*e1 + 0.5*e2));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(e1 + e2).undual(),
                GAEquals(e0 + e1 + e2 + 0.5*e3));
        REQUIRE_THAT(box1.get_outside_point<PGA3>(2*e1 + e2).undual(),
                GAEquals(e0 + e1 + 0.75*e2 + 0.5*e3));

        REQUIRE_THAT(box2.get_outside_point<PGA3>(e1).undual(),
                GAEquals(e0 + e1 + e2 + e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(-e1).undual(),
                GAEquals(e0 - e1 + e2 + e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(e2).undual(),
                GAEquals(e0 + 3*e2 + e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(-e2).undual(),
                GAEquals(e0 - e2 + e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(e3).undual(),
                GAEquals(e0 + e2 + 3*e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(-e3).undual(),
                GAEquals(e0 + e2 - e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(e1 + e2).undual(),
                GAEquals(e0 + e1 + 2*e2 + e3));
        REQUIRE_THAT(box2.get_outside_point<PGA3>(2*e1 + e2).undual(),
                GAEquals(e0 + e1 + 1.5*e2 + e3));
    }
}
