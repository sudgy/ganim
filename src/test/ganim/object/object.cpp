#include <catch2/catch_test_macros.hpp>

#include "ganim/object/object.hpp"

using namespace ganim;

namespace {
    class TestObject : public Object {
        public:
            bool on_animate = false;
            bool on_animation_start = false;
            double last_animation_t = -1;
            bool on_animation_end = false;
            Color last_change;
        private:
            virtual void on_color_changed(Color new_color) override
            {
                last_change = new_color;
            }
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
