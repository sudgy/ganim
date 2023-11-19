#include <catch2/catch_test_macros.hpp>

#include "ganim/object/animatable.hpp"

using namespace ganim;

namespace {
    class TestAnimatable : public Animatable {
        public:
            bool animated = false;
            bool animation_start = false;
            bool animation_end = false;
            double last_animation_t = -1;
            int updated = 0;
        private:
            virtual void on_animate() override
                {animated = true;}
            virtual void on_animation_start() override
                {animation_start = true;}
            virtual void update_animation(double t) override
            {
                last_animation_t = t;
                ++updated;
            }
            virtual void on_animation_end() override
                {animation_end = true;}
    };
}

TEST_CASE("Animatable updaters", "[object]") {
    auto test = TestAnimatable();
    auto updater1_count = 0;
    auto updater2_count = 0;
    auto stop_updater2 = false;
    auto updater1 = [&]{
        ++updater1_count;
    };
    auto updater2 = [&]{
        ++updater2_count;
        return !stop_updater2;
    };
    test.add_updater(updater1);
    test.add_updater(updater2);
    test.update();
    REQUIRE(updater1_count == 1);
    REQUIRE(updater2_count == 1);
    test.update();
    REQUIRE(updater1_count == 2);
    REQUIRE(updater2_count == 2);
    stop_updater2 = true;
    test.update();
    REQUIRE(updater1_count == 3);
    REQUIRE(updater2_count == 3);
    test.update();
    REQUIRE(updater1_count == 4);
    REQUIRE(updater2_count == 3);
    stop_updater2 = false;
    test.add_updater(updater2);
    test.update();
    REQUIRE(updater1_count == 5);
    REQUIRE(updater2_count == 4);
    test.clear_updaters();
    test.update();
    REQUIRE(updater1_count == 5);
    REQUIRE(updater2_count == 4);
    auto h1 = test.add_updater(updater1);
    auto h2 = test.add_updater(updater1);
    test.update();
    REQUIRE(updater1_count == 7);
    test.remove_updater(h1);
    test.update();
    REQUIRE(updater1_count == 8);
    test.remove_updater(h2);
    test.update();
    REQUIRE(updater1_count == 8);
}

TEST_CASE("Animatable animate", "[object]") {
    auto test = TestAnimatable();
    REQUIRE_THROWS(test.animate());
    test.set_fps(2);
    REQUIRE(!test.starting_animation());
    REQUIRE(!test.in_animation());
    REQUIRE(!test.animated);
    REQUIRE(!test.animation_start);
    REQUIRE(!test.animation_end);
    REQUIRE(test.last_animation_t == -1);
    REQUIRE(test.updated == 0);
    test.animate(2, [](double t){return t*t;});
    REQUIRE_THROWS(test.animate());
    REQUIRE_THROWS(test.clear_updaters());
    REQUIRE(test.starting_animation());
    REQUIRE(test.in_animation());
    REQUIRE(test.animated);
    REQUIRE(!test.animation_start);
    REQUIRE(!test.animation_end);
    REQUIRE(test.last_animation_t == -1);
    REQUIRE(test.updated == 0);
    test.update();
    REQUIRE(!test.starting_animation());
    REQUIRE(test.in_animation());
    REQUIRE(test.animated);
    REQUIRE(test.animation_start);
    REQUIRE(!test.animation_end);
    REQUIRE(test.last_animation_t == 0.0625);
    REQUIRE(test.updated == 1);
    test.update();
    REQUIRE(!test.starting_animation());
    REQUIRE(test.in_animation());
    REQUIRE(test.animated);
    REQUIRE(test.animation_start);
    REQUIRE(!test.animation_end);
    REQUIRE(test.last_animation_t == 0.25);
    REQUIRE(test.updated == 2);
    test.update();
    REQUIRE(!test.starting_animation());
    REQUIRE(test.in_animation());
    REQUIRE(test.animated);
    REQUIRE(test.animation_start);
    REQUIRE(!test.animation_end);
    REQUIRE(test.last_animation_t == 0.5625);
    REQUIRE(test.updated == 3);
    test.update();
    REQUIRE(!test.starting_animation());
    REQUIRE(!test.in_animation());
    REQUIRE(test.animated);
    REQUIRE(test.animation_start);
    REQUIRE(test.animation_end);
    REQUIRE(test.last_animation_t == 1);
    REQUIRE(test.updated == 4);
    REQUIRE_NOTHROW(test.clear_updaters());
    test.update();
    REQUIRE(!test.starting_animation());
    REQUIRE(!test.in_animation());
    REQUIRE(test.animated);
    REQUIRE(test.animation_start);
    REQUIRE(test.animation_end);
    REQUIRE(test.last_animation_t == 1);
    REQUIRE(test.updated == 4);
}

TEST_CASE("Animatable different framerates", "[object]") {
    auto test1 = TestAnimatable();
    auto test2 = TestAnimatable();
    test1.set_fps(3);
    test2.set_fps(6);
    test1.animate();
    test2.animate();
    test1.update();
    test2.update();
    test2.update();
    REQUIRE(test1.last_animation_t == test2.last_animation_t);
    test1.update();
    test2.update();
    test2.update();
    REQUIRE(test1.last_animation_t == test2.last_animation_t);
    test1.update();
    test2.update();
    test2.update();
    REQUIRE(test1.last_animation_t == test2.last_animation_t);
}
