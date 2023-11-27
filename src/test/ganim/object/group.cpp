#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/group.hpp"

#include "ganim/math.hpp"

using namespace ganim;

namespace {
    class TestDrawable : public Drawable {
        public:
            virtual void draw() override
            {
                ++draw_count;
            }
            int draw_count = 0;
    };
}

TEST_CASE("Group adding", "[object]") {
    auto leaf12 = std::array{
        Object(),
        Object()
    };
    auto& leaf1 = leaf12[0];
    auto& leaf2 = leaf12[1];
    auto leaf3 = Object();
    auto leaf4 = Object();
    auto group1 = Group();
    auto group2 = Group();
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

TEST_CASE("Group adding to scene", "[object]") {
    auto obj1 = Object();
    auto obj2 = Object();
    auto draw = TestDrawable();
    auto sub1 = Group(obj1, draw);
    auto sub2 = Group();
    auto test = Group(sub1, sub2, obj2);
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

TEST_CASE("Group movement", "[object]") {
    using namespace pga3;
    auto obj1 = Object();
    auto obj2 = Object();
    auto group = Group(obj1);
    auto test = Group(group, obj2);
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

TEST_CASE("Group color/opacity", "[object]") {
    auto obj1 = Object();
    auto obj2 = Object();
    auto group = Group(obj1);
    auto test = Group(group, obj2);
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

TEST_CASE("Group scaling", "[object]") {
    using namespace pga3;
    auto obj1 = Object();
    auto obj2 = Object();
    auto group = Group(obj1);
    auto test = Group(group, obj2);
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

TEST_CASE("Group visible", "[object]") {
    using namespace pga3;
    auto obj1 = Object();
    auto obj2 = Object();
    auto group = Group(obj1);
    auto test = Group(group, obj2);
    test.set_visible(true);
    REQUIRE(obj1.is_visible());
    REQUIRE(obj2.is_visible());
    test.set_visible(false);
    REQUIRE(!obj1.is_visible());
    REQUIRE(!obj2.is_visible());
}
