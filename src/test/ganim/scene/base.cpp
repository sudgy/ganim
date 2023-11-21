#include <catch2/catch_test_macros.hpp>

#include "test_scene.hpp"
#include "test/ganim/approx_color.hpp"
#include "ganim/object/drawable.hpp"

using namespace ganim;

namespace {
    class TestObject : public Drawable {
        public:
            virtual void draw() override
            {
                ++draw_count;
            }
            int draw_count = 0;
    };
}

TEST_CASE("Scene basics", "[scene]") {
    auto scene = TestScene(10, 10, 1, 1, 1);
    scene.frame_advance();
    scene.frame_advance();
    scene.frame_advance();
    REQUIRE(scene.time_size() == 3);
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("000000"));
    REQUIRE(scene.get_pixel(0, 8, 8) == Color("000000"));
    REQUIRE(scene.get_pixel(2, 5, 5) == Color("000000"));
}

TEST_CASE("Scene background color", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    scene.set_background_color({255, 0, 0, 255});
    scene.frame_advance();
    scene.set_background_color({0, 255, 0, 255});
    scene.frame_advance();
    scene.set_background_color({0, 0, 127, 0});
    scene.frame_advance();
    REQUIRE(scene.time_size() == 3);
    REQUIRE(scene.get_pixel(0, 0, 0) == Color("FF0000"));
    REQUIRE(scene.get_pixel(1, 0, 0) == Color("00FF00"));
    REQUIRE(scene.get_pixel(2, 0, 0) == Color("00007F"));
}

TEST_CASE("Scene waiting", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 4);
    scene.frame_advance(3);
    REQUIRE(scene.time_size() == 3);
    scene.wait(0.5);
    REQUIRE(scene.time_size() == 5);
    scene.wait(0.7);
    REQUIRE(scene.time_size() == 8);
    scene.wait(0.8);
    REQUIRE(scene.time_size() == 11);
    REQUIRE_THROWS(scene.frame_advance(-1));
    REQUIRE_THROWS(scene.wait(-0.1));
    scene.wait();
    REQUIRE(scene.time_size() == 15);
}

TEST_CASE("Scene objects", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj1 = TestObject();
    auto obj2 = TestObject();
    obj1.set_visible(true);
    obj2.set_visible(true);
    scene.add(obj1);
    REQUIRE(obj1.draw_count == 0);
    REQUIRE(obj2.draw_count == 0);
    scene.frame_advance(2);
    REQUIRE(obj1.draw_count == 2);
    REQUIRE(obj2.draw_count == 0);
    scene.add(obj2);
    REQUIRE(obj1.draw_count == 2);
    REQUIRE(obj2.draw_count == 0);
    scene.frame_advance(2);
    REQUIRE(obj1.draw_count == 4);
    REQUIRE(obj2.draw_count == 2);
}

TEST_CASE("Scene animatable updates", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = TestObject();
    obj.set_visible(true);
    scene.add(static_cast<Animatable&>(obj));
    int updated = 0;
    obj.add_updater([&]{++updated;});
    REQUIRE(obj.draw_count == 0);
    REQUIRE(updated == 0);
    scene.frame_advance(2);
    REQUIRE(obj.draw_count == 2);
    REQUIRE(updated == 2);
}

TEST_CASE("Scene drawing visible objects", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = TestObject();
    scene.add(obj);
    scene.frame_advance();
    REQUIRE(obj.draw_count == 0);
    obj.set_visible(true);
    scene.frame_advance();
    REQUIRE(obj.draw_count == 1);
    obj.set_visible(false);
    scene.frame_advance();
    REQUIRE(obj.draw_count == 1);
}
