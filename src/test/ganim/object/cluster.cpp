#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/group/cluster.hpp"

#include "ganim/math.hpp"
#include "ganim/object/shape.hpp"
#include "ganim/animation/creation.hpp"

using namespace ganim;

namespace {
    class TestObject : public SingleObject {
        public:
            virtual void draw(const Camera&) override
            {
                ++draw_count;
            }
            int draw_count = 0;
            virtual Box get_true_bounding_box() const override
                {return true_bounding_box;}
            Box true_bounding_box;
            virtual Box get_logical_bounding_box() const override
                {return logical_bounding_box;}
            Box logical_bounding_box;
    };
}

TEST_CASE("Cluster adding", "[object]") {
    auto leaf12 = std::array{
        TestObject(),
        TestObject()
    };
    auto& leaf1 = leaf12[0];
    auto& leaf2 = leaf12[1];
    auto leaf3 = TestObject();
    auto leaf4 = TestObject();
    auto group1 = Cluster();
    auto group2 = Cluster();
    group1.add(leaf12);
    group2.add(group1, leaf3);
    group2.add(leaf4);
    REQUIRE(group1.size() == 2);
    REQUIRE(group2.size() == 3);
    REQUIRE(&group1[0] == &leaf1);
    REQUIRE(&group1[1] == &leaf2);
    REQUIRE(&group2[0] == &group1);
    REQUIRE(&group2[1] == &leaf3);
    REQUIRE(&group2[2] == &leaf4);
    int i = 0;
    for (auto obj : group1) {
        if (i == 0) REQUIRE(obj == &leaf1);
        else if (i == 1) REQUIRE(obj == &leaf2);
        else REQUIRE(false);
        ++i;
    }
    i = 0;
    for (auto obj : group2) {
        if (i == 0) REQUIRE(obj == &group1);
        else if (i == 1) REQUIRE(obj == &leaf3);
        else if (i == 2) REQUIRE(obj == &leaf4);
        else REQUIRE(false);
        ++i;
    }
}

TEST_CASE("Cluster adding to scene", "[object]") {
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto draw = TestObject();
    auto sub1 = Cluster(obj1, draw);
    auto sub2 = Cluster();
    auto test = Cluster(sub1, sub2, obj2);
    int obj1_updated = 0;
    int obj2_updated = 0;
    int draw_updated = 0;
    int sub1_updated = 0;
    int sub2_updated = 0;
    int test_updated = 0;
    obj1.add_updater([&]{++obj1_updated;});
    obj2.add_updater([&]{++obj2_updated;});
    draw.add_updater([&]{++draw_updated;});
    sub1.add_updater([&]{++sub1_updated;});
    sub2.add_updater([&]{++sub2_updated;});
    test.add_updater([&]{++test_updated;});

    auto scene = TestScene(1, 1, 1, 1, 1);
    scene.add(test);
    draw.set_visible(true);
    REQUIRE(obj1_updated == 0);
    REQUIRE(obj2_updated == 0);
    REQUIRE(draw_updated == 0);
    REQUIRE(sub1_updated == 0);
    REQUIRE(sub2_updated == 0);
    REQUIRE(test_updated == 0);
    REQUIRE(draw.draw_count == 0);
    scene.frame_advance();
    REQUIRE(obj1_updated == 1);
    REQUIRE(obj2_updated == 1);
    REQUIRE(draw_updated == 1);
    REQUIRE(sub1_updated == 1);
    REQUIRE(sub2_updated == 1);
    REQUIRE(test_updated == 1);
    REQUIRE(draw.draw_count == 1);
}

TEST_CASE("Cluster movement", "[object]") {
    using namespace pga3;
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto group = Cluster(obj1);
    auto test = Cluster(group, obj2);
    obj1.shift(e1);
    obj2.shift(2*e2);
    test.shift(e1);
    REQUIRE_THAT(obj1.get_center(), GAEquals((2*e1 + e0).dual()));
    REQUIRE_THAT(obj2.get_center(), GAEquals((e1 + 2*e2 + e0).dual()));
    REQUIRE_THAT(group.get_center(), GAEquals((e1 + e0).dual()));
    REQUIRE_THAT(test.get_center(), GAEquals((e1 + e0).dual()));
    test.rotate(e12, τ/4);
    REQUIRE_THAT(obj1.get_center(), GAEquals((2*e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(obj2.get_center(), GAEquals((e2 - 2*e1 + e0).dual(), 1e-5));
    REQUIRE_THAT(group.get_center(), GAEquals((e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(test.get_center(), GAEquals((e2 + e0).dual(), 1e-5));
    test.move_to(e123);
    REQUIRE_THAT(obj1.get_center(), GAEquals((e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(obj2.get_center(), GAEquals((-2*e1 + e0).dual(), 1e-5));
    REQUIRE_THAT(group.get_center(), GAEquals(e0.dual(), 1e-5));
    REQUIRE_THAT(test.get_center(), GAEquals(e0.dual(), 1e-5));
}

TEST_CASE("Cluster color/opacity", "[object]") {
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto group = Cluster(obj1);
    auto test = Cluster(group, obj2);
    obj1.set_color("00FF00");
    obj2.set_color_with_alpha("0000FF7F");
    test.set_color("FF0000");
    REQUIRE(obj1.get_color() == "FF0000");
    REQUIRE(obj2.get_color() == "FF00007F");
    REQUIRE(test.get_color() == "FF0000");
    test.set_color_with_alpha("00FF007F");
    REQUIRE(obj1.get_color() == "00FF007F");
    REQUIRE(obj2.get_color() == "00FF007F");
    REQUIRE(test.get_color() == "00FF007F");
    obj1.set_color_with_alpha("00FF00");
    test.set_opacity(0.25);
    REQUIRE(obj1.get_color() == "00FF003F");
    REQUIRE(obj2.get_color() == "00FF003F");
    REQUIRE(test.get_color() == "00FF003F");
}

TEST_CASE("Cluster scaling", "[object]") {
    using namespace pga3;
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto group = Cluster(obj1);
    auto test = Cluster(group, obj2);
    obj1.shift(e1);
    obj2.shift(2*e2);
    test.scale(-e1, 2);
    REQUIRE(test.get_scale() == 2);
    REQUIRE_THAT(test.get_center(), GAEquals((e1 + e0).dual()));
    REQUIRE(obj1.get_scale() == 2);
    REQUIRE_THAT(obj1.get_center(), GAEquals((3*e1 + e0).dual()));
    REQUIRE(obj2.get_scale() == 2);
    REQUIRE_THAT(obj2.get_center(), GAEquals((e1 + 4*e2 + e0).dual()));
}

TEST_CASE("Cluster visible", "[object]") {
    using namespace pga3;
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto group = Cluster(obj1);
    auto test = Cluster(group, obj2);
    test.set_visible(true);
    REQUIRE(obj1.is_visible());
    REQUIRE(obj2.is_visible());
    test.set_visible(false);
    REQUIRE(!obj1.is_visible());
    REQUIRE(!obj2.is_visible());
}

TEST_CASE("Cluster draw fraction", "[object]") {
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto obj3 = TestObject();
    auto obj4 = TestObject();
    auto group1 = Cluster(obj1, obj2);
    auto group2 = Cluster(obj3, obj4);
    auto test = Cluster(group1, group2);
    group1.set_draw_subobject_ratio(0);
    //group2.set_draw_subobject_ratio(1); This should be true already
    test.set_draw_subobject_ratio(0.5);
    REQUIRE_THROWS(test.set_draw_subobject_ratio(-1));
    REQUIRE_THROWS(test.set_draw_subobject_ratio(2));

    test.set_draw_fraction(0.25);
    REQUIRE(test.get_draw_fraction() == 0.25);
    REQUIRE(group1.get_draw_fraction() == 0.375);
    REQUIRE(group2.get_draw_fraction() == 0);
    REQUIRE(obj1.get_draw_fraction() == 0.375);
    REQUIRE(obj2.get_draw_fraction() == 0.375);
    REQUIRE(obj3.get_draw_fraction() == 0);
    REQUIRE(obj4.get_draw_fraction() == 0);

    test.set_draw_fraction(0.5);
    REQUIRE(test.get_draw_fraction() == 0.5);
    REQUIRE(group1.get_draw_fraction() == 0.75);
    REQUIRE(group2.get_draw_fraction() == 0.25);
    REQUIRE(obj1.get_draw_fraction() == 0.75);
    REQUIRE(obj2.get_draw_fraction() == 0.75);
    REQUIRE(obj3.get_draw_fraction() == 0.5);
    REQUIRE(obj4.get_draw_fraction() == 0);

    test.set_draw_fraction(0.75);
    REQUIRE(test.get_draw_fraction() == 0.75);
    REQUIRE(group1.get_draw_fraction() == 1);
    REQUIRE(group2.get_draw_fraction() == 0.625);
    REQUIRE(obj1.get_draw_fraction() == 1);
    REQUIRE(obj2.get_draw_fraction() == 1);
    REQUIRE(obj3.get_draw_fraction() == 1);
    REQUIRE(obj4.get_draw_fraction() == 0.25);

    test.set_draw_fraction(0);
    REQUIRE(test.get_draw_fraction() == 0);
    REQUIRE(group1.get_draw_fraction() == 0);
    REQUIRE(group2.get_draw_fraction() == 0);
    REQUIRE(obj1.get_draw_fraction() == 0);
    REQUIRE(obj2.get_draw_fraction() == 0);
    REQUIRE(obj3.get_draw_fraction() == 0);
    REQUIRE(obj4.get_draw_fraction() == 0);

    test.set_draw_fraction(1);
    REQUIRE(test.get_draw_fraction() == 1);
    REQUIRE(group1.get_draw_fraction() == 1);
    REQUIRE(group2.get_draw_fraction() == 1);
    REQUIRE(obj1.get_draw_fraction() == 1);
    REQUIRE(obj2.get_draw_fraction() == 1);
    REQUIRE(obj3.get_draw_fraction() == 1);
    REQUIRE(obj4.get_draw_fraction() == 1);

    test = Cluster(obj1, obj2, obj3, obj4);
    test.set_draw_subobject_ratio(0.5);
    test.set_draw_fraction(0.5);
    REQUIRE(obj1.get_draw_fraction() == 1);
    REQUIRE(obj2.get_draw_fraction() == 0.75);
    REQUIRE(obj3.get_draw_fraction() == 0.25);
    REQUIRE(obj4.get_draw_fraction() == 0);
}

TEST_CASE("Cluster drawing", "[object]") {
    auto scene = TestScene(10, 10, 10, 10, 4);
    auto shape1 = Shape(
        {{ 2,  2, 0, 1},
         { 2, -2, 0, 1},
         {-2, -2, 0, 0},
         {-2,  2, 0, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape2 = Shape(
        {{4,  2, 0, 1, 1, 0, 0},
         {4, -2, 0, 1, 1, 0, 0},
         {0, -2, 0, 0, 1, 0, 0},
         {0,  2, 0, 0, 1, 0, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    auto group = Cluster(shape1, shape2);
    group.set_draw_subobject_ratio(1);
    scene.add(group);
    create(scene, group, {.rate_function = [](double t) {return t;}});
    scene.wait(1);
    REQUIRE(scene.get_pixel(0, 3, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 3, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(0, 6, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 6, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 3, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 2, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 3, 7) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 7, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 6, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 7, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 6, 7) == Color("000000"));

    REQUIRE(scene.get_pixel(1, 3, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(1, 3, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(1, 6, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(1, 6, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(1, 2, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 3, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 2, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 3, 7) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 7, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 6, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 7, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(1, 6, 7) == Color("000000"));

    REQUIRE(scene.get_pixel(2, 3, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(2, 3, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(2, 6, 3) == Color("FF0000"));
    REQUIRE(scene.get_pixel(2, 6, 6) == Color("FF0000"));
    REQUIRE(scene.get_pixel(2, 2, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 3, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 2, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 3, 7) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 7, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 6, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 7, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 6, 7) == Color("000000"));

    REQUIRE(scene.get_pixel(3, 3, 3) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(3, 3, 6) == Color("FFFFFF"));
    REQUIRE(scene.get_pixel(3, 6, 3) == Color("FF0000"));
    REQUIRE(scene.get_pixel(3, 6, 6) == Color("FF0000"));
    REQUIRE(scene.get_pixel(3, 2, 3) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 3, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 2, 6) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 3, 7) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 7, 3) == Color("FF0000"));
    REQUIRE(scene.get_pixel(3, 6, 2) == Color("000000"));
    REQUIRE(scene.get_pixel(3, 7, 6) == Color("FF0000"));
    REQUIRE(scene.get_pixel(3, 6, 7) == Color("000000"));
}

TEST_CASE("Cluster animating color", "[object]") {
    auto scene = TestScene(2, 2, 2, 2, 1);
    auto shape = Shape(
        {{ 1,  1, 0, 1},
         { 1, -1, 0, 1},
         {-1, -1, 0, 0},
         {-1,  1, 0, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    shape.set_color("#FF0000");
    auto group = Cluster(shape);
    group.set_draw_subobject_ratio(1);
    scene.add(group);
    group.set_visible(true);
    animate(scene, static_cast<Object&>(group));
    scene.wait(1);
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("FF0000"));
}

TEST_CASE("Cluster bounding box", "[object]") {
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    auto obj3 = TestObject();
    obj1.true_bounding_box = Box({0,  0, 0}, {1, 1, 0});
    obj2.true_bounding_box = Box({1,  1, 0}, {2, 2, 0});
    obj3.true_bounding_box = Box({2, -1, 0}, {3, 0, 0});
    obj1.logical_bounding_box = Box({0,  0, 0}, {0.5, 0.5, 0});
    obj2.logical_bounding_box = Box({1,  1, 0}, {1.5, 1.5, 0});
    obj3.logical_bounding_box = Box({2, -1, 0}, {2.5, 0, 0});

    auto group1 = Cluster();
    auto group2 = Cluster(obj1);
    auto group3 = Cluster(obj1, obj2);
    auto group4 = Cluster(obj1, obj2, obj3);

    REQUIRE_NOTHROW(group1.get_true_bounding_box());
    REQUIRE_NOTHROW(group1.get_logical_bounding_box());
    auto res1 = group2.get_true_bounding_box();
    auto res2 = group2.get_logical_bounding_box();
    auto res3 = group3.get_true_bounding_box();
    auto res4 = group3.get_logical_bounding_box();
    auto res5 = group4.get_true_bounding_box();
    auto res6 = group4.get_logical_bounding_box();

    using namespace vga3;
    REQUIRE_THAT(res1.p1, GAEquals(0));
    REQUIRE_THAT(res1.p2, GAEquals(e1 + e2));
    REQUIRE_THAT(res2.p1, GAEquals(0));
    REQUIRE_THAT(res2.p2, GAEquals(0.5*e1 + 0.5*e2));
    REQUIRE_THAT(res3.p1, GAEquals(0));
    REQUIRE_THAT(res3.p2, GAEquals(2*e1 + 2*e2));
    REQUIRE_THAT(res4.p1, GAEquals(0));
    REQUIRE_THAT(res4.p2, GAEquals(1.5*e1 + 1.5*e2));
    REQUIRE_THAT(res5.p1, GAEquals(-e2));
    REQUIRE_THAT(res5.p2, GAEquals(3*e1 + 2*e2));
    REQUIRE_THAT(res6.p1, GAEquals(-e2));
    REQUIRE_THAT(res6.p2, GAEquals(2.5*e1 + 1.5*e2));
}
