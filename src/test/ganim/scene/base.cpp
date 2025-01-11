#include <catch2/catch_test_macros.hpp>

#include "test_scene.hpp"
#include "test/ganim/approx_color.hpp"
#include "ganim/object/bases/single_object.hpp"
#include "ganim/object/shaders.hpp"

using namespace ganim;

namespace {
    class TestDrawable : public SingleObject {
        public:
            virtual void draw(const Camera&) override
            {
                ++draw_count;
            }
            int draw_count = 0;
            virtual Box get_original_true_bounding_box() const override
                {return Box();}
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
    auto obj1 = ObjectPtr<TestDrawable>();
    auto obj2 = ObjectPtr<TestDrawable>();
    obj1->set_visible(true);
    obj2->set_visible(true);
    scene.add(obj1);
    REQUIRE(obj1->draw_count == 0);
    REQUIRE(obj2->draw_count == 0);
    scene.frame_advance(2);
    REQUIRE(obj1->draw_count == 2);
    REQUIRE(obj2->draw_count == 0);
    scene.add(obj2);
    REQUIRE(obj1->draw_count == 2);
    REQUIRE(obj2->draw_count == 0);
    scene.frame_advance(2);
    REQUIRE(obj1->draw_count == 4);
    REQUIRE(obj2->draw_count == 2);
}

TEST_CASE("Scene animatable updates", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = ObjectPtr<TestDrawable>();
    obj->set_visible(true);
    scene.add(obj);
    int updated = 0;
    obj->add_updater([&]{++updated;});
    REQUIRE(obj->draw_count == 0);
    REQUIRE(updated == 0);
    scene.frame_advance(2);
    REQUIRE(obj->draw_count == 2);
    REQUIRE(updated == 2);
}

TEST_CASE("Scene drawing visible objects", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = ObjectPtr<TestDrawable>();
    scene.add(obj);
    scene.frame_advance();
    REQUIRE(obj->draw_count == 0);
    obj->set_visible(true);
    scene.frame_advance();
    REQUIRE(obj->draw_count == 1);
    obj->set_visible(false);
    scene.frame_advance();
    REQUIRE(obj->draw_count == 1);
}

TEST_CASE("Scene adding ranges", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto objs = std::array{
        ObjectPtr<TestDrawable>(),
        ObjectPtr<TestDrawable>()
    };
    objs[0]->set_visible(true);
    objs[1]->set_visible(true);
    scene.add(objs);
    int updated1 = 0;
    int updated2 = 0;
    objs[0]->add_updater([&]{++updated1;});
    objs[1]->add_updater([&]{++updated2;});
    REQUIRE(objs[0]->draw_count == 0);
    REQUIRE(updated1 == 0);
    REQUIRE(objs[1]->draw_count == 0);
    REQUIRE(updated2 == 0);
    scene.frame_advance(2);
    REQUIRE(objs[0]->draw_count == 2);
    REQUIRE(updated1 == 2);
    REQUIRE(objs[1]->draw_count == 2);
    REQUIRE(updated2 == 2);
}

TEST_CASE("Scene adding multiple times", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto obj = ObjectPtr<TestDrawable>();
    obj->set_visible(true);
    scene.add(obj);
    scene.add(obj);
    int updated = 0;
    obj->add_updater([&]{++updated;});
    REQUIRE(obj->draw_count == 0);
    REQUIRE(updated == 0);
    scene.frame_advance(2);
    REQUIRE(obj->draw_count == 2);
    REQUIRE(updated == 2);
}

TEST_CASE("Scene time", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 8);
    REQUIRE(scene.get_frame_count() == 0);
    REQUIRE(scene.get_time() == 0);
    scene.wait(1);
    REQUIRE(scene.get_frame_count() == 8);
    REQUIRE(scene.get_time() == 1);
    scene.wait(0.5);
    REQUIRE(scene.get_frame_count() == 12);
    REQUIRE(scene.get_time() == 1.5);
}

TEST_CASE("Scene skipping", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto test = ObjectPtr<TestDrawable>();
    test->set_visible(true);
    scene.add(test);
    scene.frame_advance();
    REQUIRE(scene.time_size() == 1);
    REQUIRE(test->draw_count == 1);
    scene.stop_animating();
    scene.frame_advance();
    REQUIRE(scene.time_size() == 1);
    REQUIRE(test->draw_count == 1);
    scene.start_animating();
    scene.frame_advance();
    REQUIRE(scene.time_size() == 2);
    REQUIRE(test->draw_count == 2);
}

TEST_CASE("Scene object removing itself in its updater", "[scene]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto test1 = ObjectPtr<Animatable>();
    auto test2 = ObjectPtr<Animatable>();
    auto test3 = ObjectPtr<Animatable>();
    scene.add(test1, test2, test3);
    test1->add_updater([&]{
        test1.reset();
    });
    bool updated = false;
    test2->add_updater([&]{
        updated = true;
    });
    scene.frame_advance();
    REQUIRE(updated);
}

TEST_CASE("Scene iterators", "[scene]") {
    auto test1 = ObjectPtr<Animatable>();
    auto test2 = ObjectPtr<TestDrawable>();
    auto test3 = ObjectPtr<TestDrawable>();
    auto scene = TestScene(1, 1, 1, 1, 1);
    scene.add(test1, test2, test3);
    bool first = true;
    for (auto obj : scene) {
        if (first) REQUIRE(obj == test2);
        else REQUIRE(obj == test3);
        first = false;
    }
}
