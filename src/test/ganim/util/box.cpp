#include <catch2/catch_test_macros.hpp>

#include "ganim/util/box.hpp"

#include "test/ganim/ga_equals.hpp"

#include "ganim/ga/exp.hpp"
#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("merge_boxes", "[object]") {
    const auto box1 = Box({0, 0, 0}, {1, 1, 1});
    const auto box2 = Box({0, 0, 0}, {3, 3, 3});
    const auto box3 = Box({1, 1, 1}, {2, 2, 2});
    const auto box4 = Box({4, 5, 6}, {5, 6, 7});
    const auto box5 = Box({1, 2, 3}, {3, 4, 5});
    const auto box6 = Box({1, 1, -1}, {2, 2, 1});

    const auto res1 = merge_boxes(box1, box2);
    const auto res2 = merge_boxes(box2, box3);
    const auto res3 = merge_boxes(box1, box4);
    const auto res4 = merge_boxes(box2, box5);
    const auto res5 = merge_boxes(box2, box6);

    using namespace vga3;
    REQUIRE_THAT(res1.p1, GAEquals(0));
    REQUIRE_THAT(res1.p2, GAEquals(3*e1 + 3*e2 + 3*e3));
    REQUIRE_THAT(res2.p1, GAEquals(0));
    REQUIRE_THAT(res2.p2, GAEquals(3*e1 + 3*e2 + 3*e3));
    REQUIRE_THAT(res3.p1, GAEquals(0));
    REQUIRE_THAT(res3.p2, GAEquals(5*e1 + 6*e2 + 7*e3));
    REQUIRE_THAT(res4.p1, GAEquals(0));
    REQUIRE_THAT(res4.p2, GAEquals(3*e1 + 4*e2 + 5*e3));
    REQUIRE_THAT(res5.p1, GAEquals(-e3));
    REQUIRE_THAT(res5.p2, GAEquals(3*e1 + 3*e2 + 3*e3));
}

TEST_CASE("transform_box", "[object]") {
    const auto box1 = Box({-1, -1, 0}, {1, 1, 0});
    const auto box2 = Box({-1, -1, -1}, {1, 1, 1});

    auto res1 = transform_box(box1, 1);
    auto res2 = transform_box(box1, 1 + pga3::e01);
    auto res3 = transform_box(box2, 1 + pga3::e01);
    auto res4 = transform_box(box1, ga_exp(pga3::e12*τ/16));
    auto res5 = transform_box(box2, ga_exp(pga3::e12*τ/16));
    auto res6 = transform_box(box1, ga_exp(pga3::e13*τ/16));
    auto res7 = transform_box(box2, ga_exp(pga3::e13*τ/16));
    auto res8 = transform_box(box1, 1, 2);
    auto res9 = transform_box(box1, 1 + pga3::e01, 2);

    using namespace vga3;

    REQUIRE_THAT(res1.p1, GAEquals(  -e1 -   e2));
    REQUIRE_THAT(res1.p2, GAEquals(   e1 +   e2));
    REQUIRE_THAT(res2.p1, GAEquals(   e1 -   e2));
    REQUIRE_THAT(res2.p2, GAEquals( 3*e1 +   e2));
    REQUIRE_THAT(res3.p1, GAEquals(   e1 -   e2 - e3));
    REQUIRE_THAT(res3.p2, GAEquals( 3*e1 +   e2 + e3));
    REQUIRE_THAT(res4.p1, GAEquals( (-e1 -   e2)*std::sqrt(2), 1e-5));
    REQUIRE_THAT(res4.p2, GAEquals( (+e1 +   e2)*std::sqrt(2), 1e-5));
    REQUIRE_THAT(res5.p1, GAEquals( (-e1 -   e2)*std::sqrt(2) - e3, 1e-5));
    REQUIRE_THAT(res5.p2, GAEquals( (+e1 +   e2)*std::sqrt(2) + e3, 1e-5));
    REQUIRE_THAT(res6.p1, GAEquals( (-e1 -   e3)/std::sqrt(2) - e2, 1e-5));
    REQUIRE_THAT(res6.p2, GAEquals( ( e1 +   e3)/std::sqrt(2) + e2, 1e-5));
    REQUIRE_THAT(res7.p1, GAEquals( (-e1 -   e3)*std::sqrt(2) - e2, 1e-5));
    REQUIRE_THAT(res7.p2, GAEquals( ( e1 +   e3)*std::sqrt(2) + e2, 1e-5));
    REQUIRE_THAT(res8.p1, GAEquals(-2*e1 - 2*e2));
    REQUIRE_THAT(res8.p2, GAEquals( 2*e1 + 2*e2));
    REQUIRE_THAT(res9.p1, GAEquals(      - 2*e2));
    REQUIRE_THAT(res9.p2, GAEquals( 4*e1 + 2*e2));
}

TEST_CASE("Box sides", "[object]") {
    const auto box = Box({1, 2, 3}, {4, 5, 6});

    const auto center = box.get_center();
    const auto left = box.get_left();
    const auto right = box.get_right();
    const auto up = box.get_up();
    const auto down = box.get_down();
    const auto out = box.get_out();
    const auto in = box.get_in();
    const auto ur = box.get_upper_right();
    const auto ul = box.get_upper_left();
    const auto lr = box.get_lower_right();
    const auto ll = box.get_lower_left();

    using namespace vga3;
    REQUIRE_THAT(center, GAEquals(2.5*e1 + 3.5*e2 + 4.5*e3));
    REQUIRE_THAT(left, GAEquals(e1 + 3.5*e2 + 4.5*e3));
    REQUIRE_THAT(right, GAEquals(4*e1 + 3.5*e2 + 4.5*e3));
    REQUIRE_THAT(up, GAEquals(2.5*e1 + 5*e2 + 4.5*e3));
    REQUIRE_THAT(down, GAEquals(2.5*e1 + 2*e2 + 4.5*e3));
    REQUIRE_THAT(out, GAEquals(2.5*e1 + 3.5*e2 + 6*e3));
    REQUIRE_THAT(in, GAEquals(2.5*e1 + 3.5*e2 + 3*e3));
    REQUIRE_THAT(ur, GAEquals(4*e1 + 5*e2 + 4.5*e3));
    REQUIRE_THAT(ul, GAEquals(1*e1 + 5*e2 + 4.5*e3));
    REQUIRE_THAT(lr, GAEquals(4*e1 + 2*e2 + 4.5*e3));
    REQUIRE_THAT(ll, GAEquals(1*e1 + 2*e2 + 4.5*e3));
}
