#include <catch2/catch_test_macros.hpp>

#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"
#include "test/ganim/ga_equals.hpp"

#include "ganim/math.hpp"
#include "ganim/object/bases/group.hpp"
#include "ganim/object/shape.hpp"
#include "ganim/animation/animation.hpp"
#include "ganim/animation/creation.hpp"

using namespace ganim;

namespace {
    class TestObject : public SingleObject {
        public:
            virtual void draw(const Camera&) override
            {
                ++draw_count;
                if (draws) draws->emplace_back(this, false);
            }
            virtual void draw_outline(const Camera&) override
            {
                if (draws) draws->emplace_back(this, true);
            }
            int draw_count = 0;
            std::vector<std::pair<TestObject*, bool>>* draws = nullptr;
            virtual Box get_original_true_bounding_box() const override
                {return true_bounding_box;}
            Box true_bounding_box;
            virtual Box get_original_logical_bounding_box() const override
                {return logical_bounding_box;}
            Box logical_bounding_box;
    };
}

TEST_CASE("Group adding", "[object]") {
    auto leaf12 = std::array{
        ObjectPtr<TestObject>(),
        ObjectPtr<TestObject>()
    };
    auto& leaf1 = leaf12[0];
    auto& leaf2 = leaf12[1];
    auto leaf3 = ObjectPtr<TestObject>();
    auto leaf4 = ObjectPtr<TestObject>();
    auto group1 = make_group();
    auto group2 = make_group();
    group1->add(leaf12);
    group2->add(group1, leaf3);
    group2->add(leaf4);
    REQUIRE(group1->size() == 2);
    REQUIRE(group2->size() == 3);
    REQUIRE(group1[0] == leaf1);
    REQUIRE(group1[1] == leaf2);
    REQUIRE(group2[0] == group1);
    REQUIRE(group2[1] == leaf3);
    REQUIRE(group2[2] == leaf4);
    int i = 0;
    for (auto obj : group1) {
        if (i == 0) REQUIRE(obj == leaf1);
        else if (i == 1) REQUIRE(obj == leaf2);
        else REQUIRE(false);
        ++i;
    }
    i = 0;
    for (auto obj : group2) {
        if (i == 0) REQUIRE(obj == group1);
        else if (i == 1) REQUIRE(obj == leaf3);
        else if (i == 2) REQUIRE(obj == leaf4);
        else REQUIRE(false);
        ++i;
    }
}

TEST_CASE("Group adding to scene", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto draw = ObjectPtr<TestObject>();
    auto sub1 = make_group(obj1, draw);
    auto sub2 = make_group();
    auto test = make_group(sub1, sub2, obj2);
    int obj1_updated = 0;
    int obj2_updated = 0;
    int draw_updated = 0;
    int sub1_updated = 0;
    int sub2_updated = 0;
    int test_updated = 0;
    obj1->add_updater([&]{++obj1_updated;});
    obj2->add_updater([&]{++obj2_updated;});
    draw->add_updater([&]{++draw_updated;});
    sub1->add_updater([&]{++sub1_updated;});
    sub2->add_updater([&]{++sub2_updated;});
    test->add_updater([&]{++test_updated;});

    auto scene = TestScene(1, 1, 1, 1, 1);
    scene.add(test);
    draw->set_visible(true);
    REQUIRE(obj1_updated == 0);
    REQUIRE(obj2_updated == 0);
    REQUIRE(draw_updated == 0);
    REQUIRE(sub1_updated == 0);
    REQUIRE(sub2_updated == 0);
    REQUIRE(test_updated == 0);
    REQUIRE(draw->draw_count == 0);
    scene.frame_advance();
    REQUIRE(obj1_updated == 1);
    REQUIRE(obj2_updated == 1);
    REQUIRE(draw_updated == 1);
    REQUIRE(sub1_updated == 1);
    REQUIRE(sub2_updated == 1);
    REQUIRE(test_updated == 1);
    REQUIRE(draw->draw_count == 1);
}

TEST_CASE("Group movement", "[object]") {
    using namespace pga3;
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto group = make_group(obj1);
    auto test = make_group(group, obj2);
    obj1->shift(e1);
    obj2->shift(2*e2);
    test->shift(e1);
    REQUIRE_THAT(obj1->get_origin(), GAEquals((2*e1 + e0).dual()));
    REQUIRE_THAT(obj2->get_origin(), GAEquals((e1 + 2*e2 + e0).dual()));
    REQUIRE_THAT(group->get_origin(), GAEquals((e1 + e0).dual()));
    REQUIRE_THAT(test->get_origin(), GAEquals((e1 + e0).dual()));
    test->rotate(τ/4, e12);
    REQUIRE_THAT(obj1->get_origin(), GAEquals((2*e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(obj2->get_origin(), GAEquals((e2 - 2*e1 + e0).dual(), 1e-5));
    REQUIRE_THAT(group->get_origin(), GAEquals((e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(test->get_origin(), GAEquals((e2 + e0).dual(), 1e-5));
    test->move_to(e123);
    REQUIRE_THAT(obj1->get_origin(), GAEquals((e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(obj2->get_origin(), GAEquals((-2*e1 + e0).dual(), 1e-5));
    REQUIRE_THAT(group->get_origin(), GAEquals(e0.dual(), 1e-5));
    REQUIRE_THAT(test->get_origin(), GAEquals(e0.dual(), 1e-5));
}

TEST_CASE("Group color/opacity", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto group = make_group(obj1);
    auto test = make_group(group, obj2);
    obj1->set_color("00FF00");
    obj2->set_color_with_alpha("0000FF7F");
    test->set_color("FF0000");
    REQUIRE(obj1->get_color() == "FF0000");
    REQUIRE(obj2->get_color() == "FF00007F");
    REQUIRE(test->get_color() == "FF0000");
    test->set_color_with_alpha("00FF007F");
    REQUIRE(obj1->get_color() == "00FF007F");
    REQUIRE(obj2->get_color() == "00FF007F");
    REQUIRE(test->get_color() == "00FF007F");
    obj1->set_color_with_alpha("00FF00");
    test->set_opacity(0.25);
    REQUIRE(obj1->get_color() == "00FF003F");
    REQUIRE(obj2->get_color() == "00FF003F");
    REQUIRE(test->get_color() == "00FF003F");
}

TEST_CASE("Group scaling", "[object]") {
    using namespace pga3;
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto group = make_group(obj1);
    auto test = make_group(group, obj2);
    obj1->shift(e1);
    obj2->shift(2*e2);
    test->scale(-e1, 2);
    REQUIRE(test->get_scale() == 2);
    REQUIRE_THAT(test->get_origin(), GAEquals((e1 + e0).dual()));
    REQUIRE(obj1->get_scale() == 2);
    REQUIRE_THAT(obj1->get_origin(), GAEquals((3*e1 + e0).dual()));
    REQUIRE(obj2->get_scale() == 2);
    REQUIRE_THAT(obj2->get_origin(), GAEquals((e1 + 4*e2 + e0).dual()));
}

TEST_CASE("Group visible", "[object]") {
    using namespace pga3;
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto group = make_group(obj1);
    auto test = make_group(group, obj2);
    test->set_visible(true);
    REQUIRE(obj1->is_visible());
    REQUIRE(obj2->is_visible());
    test->set_visible(false);
    REQUIRE(!obj1->is_visible());
    REQUIRE(!obj2->is_visible());
}

TEST_CASE("Group draw fraction", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto obj3 = ObjectPtr<TestObject>();
    auto obj4 = ObjectPtr<TestObject>();
    auto group1 = make_group(obj1, obj2);
    auto group2 = make_group(obj3, obj4);
    auto test = make_group(group1, group2);
    group1->set_draw_subobject_ratio(0);
    //group2.set_draw_subobject_ratio(1); This should be true already
    test->set_draw_subobject_ratio(0.5);
    REQUIRE_THROWS(test->set_draw_subobject_ratio(-1));
    REQUIRE_THROWS(test->set_draw_subobject_ratio(2));

    test->set_draw_fraction(0.25);
    REQUIRE(test->get_draw_fraction() == 0.25);
    REQUIRE(group1->get_draw_fraction() == 0.375);
    REQUIRE(group2->get_draw_fraction() == 0);
    REQUIRE(obj1->get_draw_fraction() == 0.375);
    REQUIRE(obj2->get_draw_fraction() == 0.375);
    REQUIRE(obj3->get_draw_fraction() == 0);
    REQUIRE(obj4->get_draw_fraction() == 0);

    test->set_draw_fraction(0.5);
    REQUIRE(test->get_draw_fraction() == 0.5);
    REQUIRE(group1->get_draw_fraction() == 0.75);
    REQUIRE(group2->get_draw_fraction() == 0.25);
    REQUIRE(obj1->get_draw_fraction() == 0.75);
    REQUIRE(obj2->get_draw_fraction() == 0.75);
    REQUIRE(obj3->get_draw_fraction() == 0.5);
    REQUIRE(obj4->get_draw_fraction() == 0);

    test->set_draw_fraction(0.75);
    REQUIRE(test->get_draw_fraction() == 0.75);
    REQUIRE(group1->get_draw_fraction() == 1);
    REQUIRE(group2->get_draw_fraction() == 0.625);
    REQUIRE(obj1->get_draw_fraction() == 1);
    REQUIRE(obj2->get_draw_fraction() == 1);
    REQUIRE(obj3->get_draw_fraction() == 1);
    REQUIRE(obj4->get_draw_fraction() == 0.25);

    test->set_draw_fraction(0);
    REQUIRE(test->get_draw_fraction() == 0);
    REQUIRE(group1->get_draw_fraction() == 0);
    REQUIRE(group2->get_draw_fraction() == 0);
    REQUIRE(obj1->get_draw_fraction() == 0);
    REQUIRE(obj2->get_draw_fraction() == 0);
    REQUIRE(obj3->get_draw_fraction() == 0);
    REQUIRE(obj4->get_draw_fraction() == 0);

    test->set_draw_fraction(1);
    REQUIRE(test->get_draw_fraction() == 1);
    REQUIRE(group1->get_draw_fraction() == 1);
    REQUIRE(group2->get_draw_fraction() == 1);
    REQUIRE(obj1->get_draw_fraction() == 1);
    REQUIRE(obj2->get_draw_fraction() == 1);
    REQUIRE(obj3->get_draw_fraction() == 1);
    REQUIRE(obj4->get_draw_fraction() == 1);

    test = make_group(obj1, obj2, obj3, obj4);
    test->set_draw_subobject_ratio(0.5);
    test->set_draw_fraction(0.5);
    REQUIRE(obj1->get_draw_fraction() == 1);
    REQUIRE(obj2->get_draw_fraction() == 0.75);
    REQUIRE(obj3->get_draw_fraction() == 0.25);
    REQUIRE(obj4->get_draw_fraction() == 0);
}

TEST_CASE("Group drawing", "[object]") {
    auto scene = TestScene(10, 10, 10, 10, 4);
    auto shape1 = make_shape(
        {{ 2,  2, 0, 1},
         { 2, -2, 0, 1},
         {-2, -2, 0, 0},
         {-2,  2, 0, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape2 = make_shape(
        {{4,  2, 0, 1, 1, 0, 0},
         {4, -2, 0, 1, 1, 0, 0},
         {0, -2, 0, 0, 1, 0, 0},
         {0,  2, 0, 0, 1, 0, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    auto group = make_group(shape1, shape2);
    group->set_draw_subobject_ratio(1);
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

TEST_CASE("Group animating color", "[object]") {
    auto scene = TestScene(2, 2, 2, 2, 1);
    auto shape = make_shape(
        {{ 1,  1, 0, 1},
         { 1, -1, 0, 1},
         {-1, -1, 0, 0},
         {-1,  1, 0, 0}},
        {0, 1, 2, 0, 2, 3}
    );
    shape->set_color("#FF0000");
    auto group = make_group(shape);
    group->set_draw_subobject_ratio(1);
    scene.add(group);
    group->set_visible(true);
    animate(scene, group);
    scene.wait(1);
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("FF0000"));
}

TEST_CASE("Group bounding box", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto obj3 = ObjectPtr<TestObject>();
    using namespace vga3;
    obj1->true_bounding_box = Box(Vec{0,  0, 0}, Vec{1, 1, 0});
    obj2->true_bounding_box = Box(Vec{0,  1, 0}, Vec{1, 2, 0});
    obj3->true_bounding_box = Box(Vec{2, -1, 0}, Vec{3, 0, 0});
    obj1->logical_bounding_box = Box(Vec{0,  0, 0}, Vec{0.5, 0.5, 0});
    obj2->logical_bounding_box = Box(Vec{0,  1, 0}, Vec{0.5, 1.5, 0});
    obj3->logical_bounding_box = Box(Vec{2, -1, 0}, Vec{2.5, 0, 0});
    obj2->shift(e1);

    auto group1 = make_group();
    auto group2 = make_group(obj1);
    auto group3 = make_group(obj1, obj2);
    auto group4 = make_group(obj1, obj2, obj3);

    REQUIRE_NOTHROW(group1->get_true_bounding_box());
    REQUIRE_NOTHROW(group1->get_logical_bounding_box());
    auto res1 = group2->get_true_bounding_box();
    auto res2 = group2->get_logical_bounding_box();
    auto res3 = group3->get_true_bounding_box();
    auto res4 = group3->get_logical_bounding_box();
    group4->shift(e1);
    auto res5 = group4->get_true_bounding_box();
    auto res6 = group4->get_logical_bounding_box();
    auto res1p1 = pga3_to_vga3(res1.get_inner_lower_left_vertex());
    auto res1p2 = pga3_to_vga3(res1.get_outer_upper_right_vertex());
    auto res2p1 = pga3_to_vga3(res2.get_inner_lower_left_vertex());
    auto res2p2 = pga3_to_vga3(res2.get_outer_upper_right_vertex());
    auto res3p1 = pga3_to_vga3(res3.get_inner_lower_left_vertex());
    auto res3p2 = pga3_to_vga3(res3.get_outer_upper_right_vertex());
    auto res4p1 = pga3_to_vga3(res4.get_inner_lower_left_vertex());
    auto res4p2 = pga3_to_vga3(res4.get_outer_upper_right_vertex());
    auto res5p1 = pga3_to_vga3(res5.get_inner_lower_left_vertex());
    auto res5p2 = pga3_to_vga3(res5.get_outer_upper_right_vertex());
    auto res6p1 = pga3_to_vga3(res6.get_inner_lower_left_vertex());
    auto res6p2 = pga3_to_vga3(res6.get_outer_upper_right_vertex());

    REQUIRE_THAT(res1p1, GAEquals(0));
    REQUIRE_THAT(res1p2, GAEquals(e1 + e2));
    REQUIRE_THAT(res2p1, GAEquals(0));
    REQUIRE_THAT(res2p2, GAEquals(0.5*e1 + 0.5*e2));
    REQUIRE_THAT(res3p1, GAEquals(0));
    REQUIRE_THAT(res3p2, GAEquals(2*e1 + 2*e2));
    REQUIRE_THAT(res4p1, GAEquals(0));
    REQUIRE_THAT(res4p2, GAEquals(1.5*e1 + 1.5*e2));
    REQUIRE_THAT(res5p1, GAEquals(e1 - e2));
    REQUIRE_THAT(res5p2, GAEquals(4*e1 + 2*e2));
    REQUIRE_THAT(res6p1, GAEquals(e1 - e2));
    REQUIRE_THAT(res6p2, GAEquals(3.5*e1 + 1.5*e2));
}

TEST_CASE("Group draw_together", "[object]") {
    auto scene = TestScene(4, 4, 4, 4, 4);
    auto shape1 = make_shape(
        {{ 0,  2, 0, 0, 1, 0, 0, 0.5},
         { 0, -2, 0, 0, 1, 0, 0, 0.5},
         {-2, -2, 0, 0, 1, 0, 0, 0.5},
         {-2,  2, 0, 0, 1, 0, 0, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape2 = make_shape(
        {{1,  2, 0, 0, 0, 1, 0, 0.5},
         {1, -2, 0, 0, 0, 1, 0, 0.5},
         {-1, -2, 0, 0, 0, 1, 0, 0.5},
         {-1,  2, 0, 0, 0, 1, 0, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto shape3 = make_shape(
        {{2,  2, 0, 0, 0, 0, 1, 0.5},
         {2, -2, 0, 0, 0, 0, 1, 0.5},
         {0, -2, 0, 0, 0, 0, 1, 0.5},
         {0,  2, 0, 0, 0, 0, 1, 0.5}},
        {0, 1, 2, 0, 2, 3}
    );
    auto group = make_group(shape1, shape2);
    group->draw_together();
    group->set_visible(true);
    shape3->set_visible(true);
    scene.add(group, shape3);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 0, 0) == ApproxColor("800000"));
    REQUIRE(scene.get_pixel(0, 1, 0) == ApproxColor("408000"));
    REQUIRE(scene.get_pixel(0, 2, 0) == ApproxColor("004080"));
    REQUIRE(scene.get_pixel(0, 3, 0) == ApproxColor("000080"));
}

TEST_CASE("Group animating subobjects", "[object]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = ObjectPtr<TestObject>();
    auto test = make_group(obj);
    test->draw_together();
    scene.add(test);
    animate(scene, obj); // Verify that the object got its fps set
}

TEST_CASE("Group subobject visibility", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto test = make_group(obj1, obj2);
    test->set_visible(true);
    auto camera = Camera(1, 1, 1);

    test->draw(camera);
    REQUIRE(test->is_visible());
    REQUIRE(obj1->is_visible());
    REQUIRE(obj2->is_visible());
    REQUIRE(obj1->draw_count == 1);
    REQUIRE(obj2->draw_count == 1);

    obj1->set_visible(false);
    test->draw(camera);
    REQUIRE(test->is_visible());
    REQUIRE(!obj1->is_visible());
    REQUIRE(obj2->is_visible());
    REQUIRE(obj1->draw_count == 1);
    REQUIRE(obj2->draw_count == 2);

    obj2->set_visible(false);
    test->draw(camera);
    REQUIRE(!test->is_visible());
    REQUIRE(!obj1->is_visible());
    REQUIRE(!obj2->is_visible());
    REQUIRE(obj1->draw_count == 1);
    REQUIRE(obj2->draw_count == 2);
}

TEST_CASE("Group outlines", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto test = make_group(obj1, obj2);
    test->set_visible(true);
    auto camera = Camera(1, 1, 1);
    auto draws = std::vector<std::pair<TestObject*, bool>>();
    obj1->draws = &draws;
    obj2->draws = &draws;
    test->draw_outline(camera);
    test->draw(camera);
    REQUIRE(draws.size() == 4);
    test->draw_together();
    test->draw_outline(camera);
    test->draw(camera);
    REQUIRE(draws.size() == 8);

    REQUIRE(draws[0].first == obj1.get());
    REQUIRE(draws[0].second);
    REQUIRE(draws[1].first == obj1.get());
    REQUIRE(!draws[1].second);
    REQUIRE(draws[2].first == obj2.get());
    REQUIRE(draws[2].second);
    REQUIRE(draws[3].first == obj2.get());
    REQUIRE(!draws[3].second);

    REQUIRE(draws[4].first == obj1.get());
    REQUIRE(draws[4].second);
    REQUIRE(draws[5].first == obj2.get());
    REQUIRE(draws[5].second);
    REQUIRE(draws[6].first == obj1.get());
    REQUIRE(!draws[6].second);
    REQUIRE(draws[7].first == obj2.get());
    REQUIRE(!draws[7].second);
}

TEST_CASE("Group ranges", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto obj3 = ObjectPtr<TestObject>();
    auto test = make_group(obj1, obj2, obj3);
    test->set_draw_subobject_ratio(0.2);
    REQUIRE_THROWS(test->range(0, 4));
    REQUIRE_THROWS(test->range(-6, -4));
    REQUIRE_THROWS(test->range(-6));
    REQUIRE_THROWS(test->range(3));
    auto test2 = test->range(0, 2);
    auto test3 = test->range(-1, 3);
    REQUIRE(test2->size() == 2);
    REQUIRE(test3->size() == 1);
    REQUIRE(test2[0] == obj1);
    REQUIRE(test2[1] == obj2);
    REQUIRE(test3[0] == obj3);
    REQUIRE(test2->get_draw_subobject_ratio() == 0.2);
    REQUIRE(test3->get_draw_subobject_ratio() == 0.2);
    REQUIRE(!test2->drawing_together());
    REQUIRE(!test3->drawing_together());

    test->set_draw_subobject_ratio(0.3);
    test->draw_together();
    auto test4 = test->range(0, -2);
    auto test5 = test->range(1);
    REQUIRE(test4->size() == 1);
    REQUIRE(test5->size() == 2);
    REQUIRE(test4[0] == obj1);
    REQUIRE(test5[0] == obj2);
    REQUIRE(test5[1] == obj3);
    REQUIRE(test4->get_draw_subobject_ratio() == 0.3);
    REQUIRE(test5->get_draw_subobject_ratio() == 0.3);
    REQUIRE(test4->drawing_together());
    REQUIRE(test5->drawing_together());
}

TEST_CASE("Group align_by_subobject", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto obj3 = ObjectPtr<TestObject>();
    auto box = Box(vga2::Vec(0, 0), vga2::Vec(1, 1));
    obj1->logical_bounding_box = box;
    obj2->logical_bounding_box = box;
    obj3->logical_bounding_box = box;

    auto group = make_group(obj1, obj2);
    using namespace pga2;
    obj2->shift(2*e1);
    group->shift(e2 - e1);
    group->align_by_subobject(1, *obj3, -e1);
    REQUIRE_THAT(obj1->get_center().undual(), GAEquals(e0 - 1.5*e1 + 1.5*e2));
    REQUIRE_THAT(obj2->get_center().undual(), GAEquals(e0 + 0.5*e1 + 1.5*e2));
    REQUIRE_THAT(obj3->get_center().undual(), GAEquals(e0 + 0.5*e1 + 0.5*e2));
    group->align_by_subobject(0, *obj3, e1, e2);
    REQUIRE_THAT(obj1->get_center().undual(), GAEquals(e0 + 0.5*e1 + 0.5*e2));
    REQUIRE_THAT(obj2->get_center().undual(), GAEquals(e0 + 2.5*e1 + 0.5*e2));
    REQUIRE_THAT(obj3->get_center().undual(), GAEquals(e0 + 0.5*e1 + 0.5*e2));
}

TEST_CASE("Group arranging", "[object]") {
    auto obj1 = ObjectPtr<TestObject>();
    auto obj2 = ObjectPtr<TestObject>();
    auto obj3 = ObjectPtr<TestObject>();
    auto obj4 = ObjectPtr<TestObject>();
    auto group = make_group(obj1, obj2, obj3, obj4);
    using namespace vga2;
    auto simple_box = Box(0*e1, e1 + e2);
    obj1->logical_bounding_box = simple_box;
    obj2->logical_bounding_box = simple_box;
    obj3->logical_bounding_box = simple_box;
    obj4->logical_bounding_box = simple_box;
    auto args = ArrangeArgs{.buff = 0.5};
    group->arrange_down(args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(2.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(-0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(-2.25*e2));

    auto bigger_box = Box(0*e1, 2*e1 + 2*e2);
    obj1->logical_bounding_box = bigger_box;
    group->arrange_down(args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(2.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(0.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(-1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(-2.75*e2));

    group->shift(e1);
    obj1->shift(2*e1 + e2);
    obj2->shift(3*e1 - e2);
    obj3->shift(4*e1 + e2);
    obj4->shift(5*e1 - e2);
    group->arrange_down(args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(e1 + 2.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(e1 + 0.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(e1 + -1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(e1 + -2.75*e2));

    group->arrange_down({.buff = 0.5, .align = e1});
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(e1 + 2.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(1.5*e1 + 0.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(1.5*e1 + -1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(1.5*e1 + -2.75*e2));

    group->arrange_right(args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(-1.25*e1));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(0.75*e1));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(2.25*e1));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(3.75*e1));

    group->arrange_in_grid(2, args, args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(0.25*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(2.25*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(0.25*e1 - 1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(2.25*e1 - 1.25*e2));

    group->arrange_in_grid(2, {.buff = 1}, args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(2.5*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(-1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(2.5*e1 - 1.25*e2));

    group->arrange_in_grid(
        2,
        {.buff = 0.5, .align = -e1},
        {.buff = 0.5, .align = e2}
    );
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(0.25*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(2.25*e1 + 1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(-0.25*e1 - 1.25*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(2.25*e1 - 1.25*e2));

    group->arrange_in_grid(3, args, args);
    REQUIRE_THAT(pga2_to_vga2(obj1->get_center()), GAEquals(-0.5*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj2->get_center()), GAEquals(1.5*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj3->get_center()), GAEquals(3*e1 + 0.75*e2));
    REQUIRE_THAT(pga2_to_vga2(obj4->get_center()), GAEquals(-0.5*e1 - 1.25*e2));
}
