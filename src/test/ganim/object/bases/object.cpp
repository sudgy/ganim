#include <catch2/catch_test_macros.hpp>

#include "ganim/object/bases/object.hpp"

#include "ganim/animation/animation.hpp"
#include "ganim/animation/color_stack.hpp"
#include "test/ganim/ga_equals.hpp"
#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

namespace {
    class TestObject : public Object {
        public:
            Color last_change;
            pga3::Trivec scaled_point;
            double last_scale = -1;
            bool last_visible = false;
            Box bounding_box;

            virtual TestObject& set_visible(bool visible) override
            {
                Object::set_visible(visible);
                last_visible = visible;
                return *this;
            }
            virtual TestObject& set_color(Color new_color) override
            {
                Object::set_color(new_color);
                last_change = new_color;
                return *this;
            }
            using Object::scale;
            virtual TestObject& scale(
                double amount,
                const pga3::Trivec& about_point
            ) override
            {
                Object::scale(amount, about_point);
                scaled_point = about_point;
                last_scale = amount;
                return *this;
            }
            std::unique_ptr<TestObject> anim_copy() const
                {return std::make_unique<TestObject>(*this);}
            virtual Box get_original_true_bounding_box() const override
                {return bounding_box;}
            virtual void draw(const Camera&) override {}
            virtual void draw_outline(const Camera&) override {}
            virtual void set_outline(const Color&, double, bool) override {}
            virtual void invalidate_outline() override {}
            virtual Color get_outline_color() const override {return Color();}
            virtual double get_outline_thickness() const override {return 0.0;}
    };
}

TEST_CASE("Object color/opacity", "[object]") {
    auto test = TestObject();
    REQUIRE(test.get_color() == Color("FFFFFF"));
    REQUIRE(test.get_opacity() == 1);
    test.set_color(Color("AAAAAA"));
    REQUIRE(test.get_color() == Color("AAAAAAFF"));
    REQUIRE(test.last_change == Color("AAAAAAFF"));
    REQUIRE(test.get_opacity() == 1);
    test.set_color(Color("BBBBBBBB"));
    REQUIRE(test.get_color() == Color("BBBBBBBB"));
    REQUIRE(test.last_change == Color("BBBBBBBB"));
    REQUIRE(test.get_opacity() == 1);
    test.set_opacity(0.5);
    REQUIRE(test.get_color() == Color("BBBBBBBB"));
    REQUIRE(test.last_change == Color("BBBBBBBB"));
    REQUIRE(test.get_opacity() == 0.5);
}

TEST_CASE("Object animating color", "[object]") {
    auto test = ObjectPtr<TestObject>();
    auto scene = TestScene(1, 1, 1, 1, 4);
    test->set_fps(4);
    animate(scene, test, {.rate_function = [](double t){return t;}})
        .set_color("000000");
    REQUIRE(test->get_color() == Color("FFFFFF"));
    test->update();
    REQUIRE(test->get_color() == Color("BFBFBF"));
    REQUIRE(test->last_change == Color("BFBFBF"));
    test->update();
    REQUIRE(test->get_color() == Color("7F7F7F"));
    REQUIRE(test->last_change == Color("7F7F7F"));
    test->update();
    REQUIRE(test->get_color() == Color("3F3F3F"));
    REQUIRE(test->last_change == Color("3F3F3F"));
    test->update();
    REQUIRE(test->get_color() == Color("000000"));
    REQUIRE(test->last_change == Color("000000"));
    animate(scene, test, {.rate_function = [](double t){return t;}})
        .set_color("0000007F");
    REQUIRE(test->get_color() == Color("000000FF"));
    test->update();
    REQUIRE(test->get_color() == Color("000000DF"));
    REQUIRE(test->last_change == Color("000000DF"));
    test->update();
    REQUIRE(test->get_color() == Color("000000BF"));
    REQUIRE(test->last_change == Color("000000BF"));
    test->update();
    REQUIRE(test->get_color() == Color("0000009F"));
    REQUIRE(test->last_change == Color("0000009F"));
    test->update();
    REQUIRE(test->get_color() == Color("0000007F"));
    REQUIRE(test->last_change == Color("0000007F"));
}

TEST_CASE("Object scaling", "[object]") {
    using namespace pga3;
    auto test = TestObject();
    auto get_p = [&](pga3::Trivec p){
        p += -p.blade_project<e123>()*e123;
        p *= test.get_scale();
        p += e123;
        return ~test.get_rotor()*p*test.get_rotor();
    };
    REQUIRE(test.get_scale() == 1);

    test.scale(2);
    REQUIRE_THAT(test.scaled_point, GAEquals(e123));
    REQUIRE(test.last_scale == 2);
    REQUIRE_THAT(get_p(e123), GAEquals(e123));
    REQUIRE_THAT(get_p((e1 + e0).dual()), GAEquals((2*e1 + e0).dual()));
    REQUIRE(test.get_scale() == 2);

    test.last_scale = 1;
    test.scale(2, vga2::e1);
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e0).dual()));
    REQUIRE(test.last_scale == 2);
    REQUIRE_THAT(get_p(e123), GAEquals((-e1 + e0).dual()));
    REQUIRE_THAT(get_p((e1 + e0).dual()), GAEquals((3*e1 + e0).dual()));
    REQUIRE(test.get_scale() == 4);

    test.last_scale = 1;
    test.scale(2);
    REQUIRE_THAT(test.scaled_point, GAEquals((-e1 + e0).dual()));
    REQUIRE(test.last_scale == 2);
    REQUIRE_THAT(get_p(e123), GAEquals((-e1 + e0).dual()));
    REQUIRE_THAT(get_p((e1 + e0).dual()), GAEquals((7*e1 + e0).dual()));
    REQUIRE(test.get_scale() == 8);

    test.last_scale = 1;
    test.scale(2, vga3::e3);
    REQUIRE_THAT(test.scaled_point, GAEquals((e3 + e0).dual()));
    REQUIRE(test.last_scale == 2);
    REQUIRE_THAT(get_p(e123), GAEquals((-2*e1 - e3 + e0).dual()));
    REQUIRE_THAT(get_p((e1 + e0).dual()), GAEquals((14*e1 - e3 + e0).dual()));

    test.scale(3, pga2::e1);
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e0).dual()));
    REQUIRE(test.last_scale == 3);
    test.scale(2, pga3::e2);
    REQUIRE_THAT(test.scaled_point, GAEquals((e2 + e0).dual()));
    REQUIRE(test.last_scale == 2);
    test.scale(3, (pga2::e1 + pga2::e0).dual());
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e0).dual()));
    REQUIRE(test.last_scale == 3);
    test.scale(2, (pga3::e2 + pga3::e0).dual());
    REQUIRE_THAT(test.scaled_point, GAEquals((e2 + e0).dual()));
    REQUIRE(test.last_scale == 2);
}

TEST_CASE("Object animating scale", "[object]") {
    using namespace pga3;
    auto test = ObjectPtr<TestObject>();
    auto scene = TestScene(1, 1, 1, 1, 4);
    test->set_fps(4);
    test->shift(e1);
    auto get_p = [&](pga3::Trivec p){
        p += -p.blade_project<e123>()*e123;
        p *= test->get_scale();
        p += e123;
        return ~test->get_rotor()*p*test->get_rotor();
    };
    animate(scene, test, {.rate_function = [](double t){return t;}})
        .scale(2, e1 + e2);
    auto p = (-e1 + e0).dual();
    REQUIRE_THAT(get_p(p), GAEquals(e123));
    test->update();
    REQUIRE_THAT(get_p(p), GAEquals((-0.25*e1 - 0.25*e2 + e0).dual()));
    REQUIRE(test->last_scale == 1.25 / 1.0);
    test->update();
    REQUIRE_THAT(get_p(p), GAEquals((-0.5*e1 - 0.5*e2 + e0).dual()));
    REQUIRE(test->last_scale == 1.5 / 1.25);
    test->update();
    REQUIRE_THAT(get_p(p), GAEquals((-0.75*e1 - 0.75*e2 + e0).dual()));
    REQUIRE(test->last_scale == 1.75 / 1.5);
    test->update();
    REQUIRE_THAT(get_p(p), GAEquals((-e1 - e2 + e0).dual()));
    REQUIRE(test->last_scale == 2.0 / 1.75);
    test->update();
    REQUIRE_THAT(get_p(p), GAEquals((-e1 - e2 + e0).dual()));
    REQUIRE(test->last_scale == 2.0 / 1.75);
}

TEST_CASE("Object visibile", "[object]") {
    auto test = TestObject();
    REQUIRE(!test.is_visible());
    REQUIRE(!test.last_visible);
    test.set_visible(true);
    REQUIRE(test.is_visible());
    REQUIRE(test.last_visible);
    test.set_visible(false);
    REQUIRE(!test.is_visible());
    REQUIRE(!test.last_visible);
}

TEST_CASE("Object interpolate", "[object]") {
    auto test1 = TestObject();
    auto test2 = TestObject();
    auto test3 = TestObject();
    test2.set_color("00000000");
    test2.scale(2);
    test2.set_draw_fraction(0.25);
    test3.set_color("FFFFFFFF");
    test3.scale(6);
    test3.set_draw_fraction(0.75);
    test1.interpolate(test2, test3, 0.25);
    REQUIRE(test1.get_color() == "3F3F3F3F");
    REQUIRE(test1.get_scale() == 3);
    REQUIRE(test1.get_draw_fraction() == 0.375);
    test1.interpolate(test2, test3, 0.5);
    REQUIRE(test1.get_color() == "7F7F7F7F");
    REQUIRE(test1.get_scale() == 4);
    REQUIRE(test1.get_draw_fraction() == 0.5);
    test1.interpolate(test2, test3, 0.75);
    REQUIRE(test1.get_color() == "BFBFBFBF");
    REQUIRE(test1.get_scale() == 5);
    REQUIRE(test1.get_draw_fraction() == 0.625);
    test1.interpolate(test2, test3, 1);
    REQUIRE(test1.get_color() == "FFFFFFFF");
    REQUIRE(test1.get_scale() == 6);
    REQUIRE(test1.get_draw_fraction() == 0.75);
}

TEST_CASE("Object Box wrappers", "[object]") {
    auto test = TestObject();
    test.bounding_box = Box(vga3::Vec(-1, -1, -1), vga3::Vec(1, 1, 1));
    using namespace pga2;
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0));
    REQUIRE_THAT(test.get_right().undual(), GAEquals(e0 + e1));

    test.shift(e1);
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0 + e1));
    REQUIRE_THAT(test.get_right().undual(), GAEquals(e0 + 2*e1));
}

TEST_CASE("Object next_to", "[object]") {
    auto test1 = TestObject();
    auto test2 = TestObject();
    test1.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(1, 1, 1));
    test2.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(1, 1, 1));
    using namespace pga2;

    test2.next_to(e12, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 0.75*e1));

    test2.next_to(test1, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 1.75*e1 + 0.5*e2));

    test2.next_to(test1, e2, 0.5);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 0.5*e1 + 2*e2));

    test2.next_to(test1, e1 + e2);
    constexpr auto p = 1.676776695297;
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + p*e1 + p*e2, 1e-5));

    test2.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(2, 2, 2));

    test2.next_to(e12, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 1.25*e1, 1e-5));

    test2.next_to(test1, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 2.25*e1 + 0.5*e2));

    test2.next_to(test1, e2, 0.5);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 0.5*e1 + 2.5*e2));

    test2.next_to(test1, e1 + e2);
    constexpr auto q = p + 0.5;
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + q*e1 + q*e2, 1e-5));
}

TEST_CASE("Object align_to", "[object]") {
    auto test1 = TestObject();
    auto test2 = TestObject();
    test1.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(1, 1, 1));
    test2.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(1, 1, 1));
    using namespace pga2;

    test2.align_to(e12, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 - 0.5*e1 + 0.5*e2));

    test2.align_to(test1, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 0.5*e1 + 0.5*e2));

    test2.shift(e1 + e2);
    test2.align_to(test1, e2);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 1.5*e1 + 0.5*e2));

    test2.align_to(test1, e1 + e2);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + e1));

    test2.align_to(test1, e1, e2);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 0.5*e1 + 0.5*e2));

    test2.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(2, 2, 2));

    test2.align_to(e12, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 - e1 + e2));

    test2.align_to(test1, e1);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + e2));

    test2.shift(e1 + e2);
    test2.align_to(test1, e2);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + e1));

    test2.align_to(test1, e1 + e2);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0 + 0.5*e1 - 0.5*e2));

    test2.align_to(test1, e1, e2);
    REQUIRE_THAT(test2.get_center().undual(), GAEquals(e0));
}

TEST_CASE("Object to_edge", "[object]") {
    using namespace pga2;
    auto test = TestObject();
    test.bounding_box = Box(vga3::Vec(0, 0, 0), vga3::Vec(2, 2, 2));
    auto camera = Camera(20, 10, 8);

    test.to_edge(camera, e1, 0);
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0 + 4*e1));

    test.to_edge(camera, e1, 1);
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0 + 3*e1));

    test.to_edge(camera, e2, 1);
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0 + 2*e2));

    test.to_edge(camera, e1 + e2, 1);
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0 + 3*e1 + 2*e2));

    test.to_edge(camera, e1 + 2*e2, 1);
    REQUIRE_THAT(test.get_center().undual(), GAEquals(e0 + 1.5*e1 + 2*e2));
}

TEST_CASE("Object color stack", "[object]") {
    auto test = TestObject();

    auto red = Color("FF0000");
    auto green = Color("00FF00");
    auto blue = Color("0000FF");
    test.set_color(red);
    test.push_color(green);
    REQUIRE(test.get_color() == green);
    test.set_color(blue);
    REQUIRE(test.get_color() == blue);
    test.pop_color();
    REQUIRE(test.get_color() == red);
    test.pop_color();
    REQUIRE(test.get_color() == red);
}

TEST_CASE("Object color stack animating", "[object]") {
    auto test1 = ObjectPtr<TestObject>();
    auto test2 = ObjectPtr<TestObject>();
    auto scene1 = TestScene(1, 1, 1, 1, 4);
    auto scene2 = TestScene(1, 1, 1, 1, 4);

    auto red = Color("FF0000");
    auto green = Color("00FF00");
    auto blue = Color("0000FF");

    animate(scene1, test1).push_color(red);
    REQUIRE_THROWS(scene1.wait(1));

    test2->set_color(red);
    push_color(scene2, test2, green, {.rate_function = [](double t){return t;}});
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "BF3F00");
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "7F7F00");
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "3FBF00");
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "00FF00");

    test2->set_color(blue);
    pop_color(scene2, test2, {.rate_function = [](double t){return t;}});
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "3F00BF");
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "7F007F");
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "BF003F");
    scene2.frame_advance();
    REQUIRE(test2->get_color() == "FF0000");
}
