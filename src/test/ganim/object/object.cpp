#include <catch2/catch_test_macros.hpp>

#include "ganim/object/object.hpp"

#include "test/ganim/ga_equals.hpp"

using namespace ganim;

namespace {
    class TestObject : public Object {
        public:
            bool on_animate = false;
            bool on_animation_start = false;
            double last_animation_t = -1;
            bool on_animation_end = false;
            Color last_change;
            pga3::Trivector scaled_point;
            double last_scale = -1;
            bool last_visible = false;

            virtual TestObject& set_visible(bool visible) override
            {
                Object::set_visible(visible);
                last_visible = visible;
                return *this;
            }
            virtual TestObject& set_color_with_alpha(Color new_color) override
            {
                Object::set_color_with_alpha(new_color);
                last_change = new_color;
                return *this;
            }
            using Object::scale;
            virtual TestObject& scale(
                const pga3::Trivector& about_point,
                double amount
            ) override
            {
                Object::scale(about_point, amount);
                scaled_point = about_point;
                last_scale = amount;
                return *this;
            }

        private:
            virtual void object_on_animate() override
            {
                on_animate = true;
            }
            virtual void object_on_animation_start() override
            {
                on_animation_start = true;
            }
            virtual void object_update_animation(double t) override
            {
                last_animation_t = t;
            }
            virtual void object_on_animation_end() override
            {
                on_animation_end = true;
            }
    };
}

TEST_CASE("Object color", "[object]") {
    auto test = TestObject();
    REQUIRE(test.get_color() == Color("FFFFFF"));
    test.set_color(Color("AAAAAAAA"));
    REQUIRE(test.get_color() == Color("AAAAAAFF"));
    REQUIRE(test.last_change == Color("AAAAAAFF"));
    test.set_color_with_alpha(Color("BBBBBBBB"));
    REQUIRE(test.get_color() == Color("BBBBBBBB"));
    REQUIRE(test.last_change == Color("BBBBBBBB"));
    test.set_opacity(0.5);
    REQUIRE(test.get_color() == Color("BBBBBB7F"));
    REQUIRE(test.last_change == Color("BBBBBB7F"));
}

TEST_CASE("Object animating color", "[object]") {
    auto test = TestObject();
    test.set_fps(4);
    test.animate([](double t){return t;}).set_color("000000");
    REQUIRE(test.get_color() == Color("FFFFFF"));
    test.update();
    REQUIRE(test.get_color() == Color("BFBFBF"));
    REQUIRE(test.last_change == Color("BFBFBF"));
    test.update();
    REQUIRE(test.get_color() == Color("7F7F7F"));
    REQUIRE(test.last_change == Color("7F7F7F"));
    test.update();
    REQUIRE(test.get_color() == Color("3F3F3F"));
    REQUIRE(test.last_change == Color("3F3F3F"));
    test.update();
    REQUIRE(test.get_color() == Color("000000"));
    REQUIRE(test.last_change == Color("000000"));
    test.animate([](double t){return t;}).set_opacity(0.5);
    REQUIRE(test.get_color() == Color("000000FF"));
    test.update();
    REQUIRE(test.get_color() == Color("000000DF"));
    REQUIRE(test.last_change == Color("000000DF"));
    test.update();
    REQUIRE(test.get_color() == Color("000000BF"));
    REQUIRE(test.last_change == Color("000000BF"));
    test.update();
    REQUIRE(test.get_color() == Color("0000009F"));
    REQUIRE(test.last_change == Color("0000009F"));
    test.update();
    REQUIRE(test.get_color() == Color("0000007F"));
    REQUIRE(test.last_change == Color("0000007F"));
}

TEST_CASE("Object sub animations", "[object]") {
    auto test = TestObject();
    test.set_fps(2);
    test.animate([](double t){return t;});
    REQUIRE(test.on_animate);
    REQUIRE(!test.on_animation_start);
    REQUIRE(test.last_animation_t == -1);
    REQUIRE(!test.on_animation_end);
    test.update();
    REQUIRE(test.on_animate);
    REQUIRE(test.on_animation_start);
    REQUIRE(test.last_animation_t == 0.5);
    REQUIRE(!test.on_animation_end);
    test.update();
    REQUIRE(test.on_animate);
    REQUIRE(test.on_animation_start);
    REQUIRE(test.last_animation_t == 1);
    REQUIRE(test.on_animation_end);
}

TEST_CASE("Object scaling", "[object]") {
    using namespace pga3;
    auto test = TestObject();
    auto get_p = [&](pga3::Trivector p){
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
    test.scale(vga2::e1, 2);
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
    test.scale(vga3::e3, 2);
    REQUIRE_THAT(test.scaled_point, GAEquals((e3 + e0).dual()));
    REQUIRE(test.last_scale == 2);
    REQUIRE_THAT(get_p(e123), GAEquals((-2*e1 - e3 + e0).dual()));
    REQUIRE_THAT(get_p((e1 + e0).dual()), GAEquals((14*e1 - e3 + e0).dual()));

    test.scale(pga2::e1, 3);
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e0).dual()));
    REQUIRE(test.last_scale == 3);
    test.scale(pga3::e2, 2);
    REQUIRE_THAT(test.scaled_point, GAEquals((e2 + e0).dual()));
    REQUIRE(test.last_scale == 2);
    test.scale((pga2::e1 + pga2::e0).dual(), 3);
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e0).dual()));
    REQUIRE(test.last_scale == 3);
    test.scale((pga3::e2 + pga3::e0).dual(), 2);
    REQUIRE_THAT(test.scaled_point, GAEquals((e2 + e0).dual()));
    REQUIRE(test.last_scale == 2);
}

TEST_CASE("Object animating scale", "[object]") {
    using namespace pga3;
    auto test = TestObject();
    test.set_fps(4);
    test.shift(e1);
    auto get_p = [&](pga3::Trivector p){
        p += -p.blade_project<e123>()*e123;
        p *= test.get_scale();
        p += e123;
        return ~test.get_rotor()*p*test.get_rotor();
    };
    test.animate([](double t){return t;}).scale(e1 + e2, 2);
    auto p = (-e1 + e0).dual();
    REQUIRE_THAT(get_p(p), GAEquals(e123));
    test.update();
    REQUIRE_THAT(get_p(p), GAEquals((-0.25*e1 - 0.25*e2 + e0).dual()));
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e2 + e0).dual()));
    //REQUIRE(test.last_scale == 1.25 / 1.0);
    test.update();
    REQUIRE_THAT(get_p(p), GAEquals((-0.5*e1 - 0.5*e2 + e0).dual()));
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e2 + e0).dual()));
    //REQUIRE(test.last_scale == 1.5 / 1.25);
    test.update();
    REQUIRE_THAT(get_p(p), GAEquals((-0.75*e1 - 0.75*e2 + e0).dual()));
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e2 + e0).dual()));
    //REQUIRE(test.last_scale == 1.75 / 1.5);
    test.update();
    REQUIRE_THAT(get_p(p), GAEquals((-e1 - e2 + e0).dual()));
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e2 + e0).dual()));
    //REQUIRE(test.last_scale == 2.0 / 1.75);
    test.update();
    REQUIRE_THAT(get_p(p), GAEquals((-e1 - e2 + e0).dual()));
    REQUIRE_THAT(test.scaled_point, GAEquals((e1 + e2 + e0).dual()));
    //REQUIRE(test.last_scale == 2.0 / 1.75);
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

TEST_CASE("Object fading", "[object]") {
    auto test = TestObject();
    test.set_fps(4);
    test.fade_in();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x0);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xBF);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xFF);
    test.fade_out();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xFF);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0xBF);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0xFF);

    test.set_opacity(0.5);
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.fade_in(0.5);
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x0);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.fade_out(0.5);
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
    test.update();
    REQUIRE(test.is_visible());
    REQUIRE(test.get_color().a == 0x3F);
    test.update();
    REQUIRE(!test.is_visible());
    REQUIRE(test.get_color().a == 0x7F);
}

TEST_CASE("Object interpolate", "[object]") {
    auto test1 = TestObject();
    auto test2 = TestObject();
    auto test3 = TestObject();
    test2.set_color_with_alpha("00000000");
    test2.scale(2);
    test2.set_draw_fraction(0.25);
    test3.set_color_with_alpha("FFFFFFFF");
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
