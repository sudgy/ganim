#include <catch2/catch_test_macros.hpp>

#include "ganim/animation/animation.hpp"
#include "ganim/object/bases/group.hpp"

#include "test/ganim/scene/test_scene.hpp"

using namespace ganim;

namespace {
    class TestAnimatable : public Animatable {
        public:
            ~TestAnimatable()
            {
                if (on_delete) *on_delete = 1;
            }
            std::unique_ptr<TestAnimatable> anim_copy() const
                {return std::make_unique<TestAnimatable>(*this);}
            void interpolate(
                const TestAnimatable& start,
                const TestAnimatable& end,
                double t
            )
            {
                last_start = &start;
                last_end = &end;
                last_t = t;
                ++updated;
                end.self = this;
            }
            const TestAnimatable* last_start = nullptr;
            const TestAnimatable* last_end = nullptr;
            double last_t = -1;
            int updated = 0;
            mutable TestAnimatable* self = nullptr;
            int* on_delete = nullptr;
    };
    class TestAnimatable2 : public TestAnimatable {};
    class TestAnimatable3 : public TestAnimatable {
        public:
            std::unique_ptr<TestAnimatable3> anim_copy() const
                {return std::make_unique<TestAnimatable3>(*this);}
    };
    class TestObject : public Object {
        public:
            virtual Box get_true_bounding_box() const override {return Box();}
            virtual void draw(const Camera&) override {}
            virtual void draw_outline(const Camera&) override {}
            virtual void set_outline(const Color&, double) override {}
            virtual void invalidate_outline() override {}
            virtual Color get_outline_color() const override {return Color();}
            virtual double get_outline_thickness() const override {return 0.0;}
    };
}

static_assert(animatable<TestAnimatable>);
static_assert(animatable<TestAnimatable2>);
static_assert(animatable<TestAnimatable3>);
static_assert(std::is_same_v<
        decltype(std::declval<Animation<TestAnimatable3>>()
                .get_starting_object()),
        TestAnimatable3&>);

TEST_CASE("Animation basics", "[animation]") {
    auto test = TestAnimatable();
    auto scene = TestScene(1, 1, 1, 1, 2);
    test.set_fps(2);
    auto anim = Animation(
        scene,
        MaybeOwningRef(test),
        {2, [](double t){return t*t;}}
    );
    auto& start = anim.get_starting_object();
    auto& end = anim.get_ending_object();
    bool ended = false;
    anim.at_end([&]{ended = true;});
    test.add_updater(std::move(anim));
    REQUIRE(test.last_start == nullptr);
    REQUIRE(test.last_end == nullptr);
    REQUIRE(test.last_t == -1);
    REQUIRE(test.updated == 0);
    REQUIRE(!ended);
    test.update();
    REQUIRE(test.last_start == &start);
    REQUIRE(test.last_end == &end);
    REQUIRE(test.last_t == 0.0625);
    REQUIRE(test.updated == 1);
    REQUIRE(!ended);
    test.update();
    REQUIRE(test.last_start == &start);
    REQUIRE(test.last_end == &end);
    REQUIRE(test.last_t == 0.25);
    REQUIRE(test.updated == 2);
    REQUIRE(!ended);
    test.update();
    REQUIRE(test.last_start == &start);
    REQUIRE(test.last_end == &end);
    REQUIRE(test.last_t == 0.5625);
    REQUIRE(test.updated == 3);
    REQUIRE(!ended);
    test.update();
    REQUIRE(test.last_start == &start);
    REQUIRE(test.last_end == &end);
    REQUIRE(test.last_t == 1);
    REQUIRE(test.updated == 4);
    REQUIRE(ended);
    test.update();
    REQUIRE(test.last_start == &start);
    REQUIRE(test.last_end == &end);
    REQUIRE(test.last_t == 1);
    REQUIRE(test.updated == 4);
    REQUIRE(ended);
}

TEST_CASE("Animation with rvalues", "[animation]") {
    auto scene = TestScene(1, 1, 1, 1, 1);
    auto& test = animate(
        scene,
        TestAnimatable(),
        {.duration = 4, .rate_function = [](double x){return x;}}
    );
    scene.frame_advance();
    REQUIRE(test.self);
    auto& test2 = *test.self;
    int on_delete = 0;
    test2.on_delete = &on_delete;
    REQUIRE(test2.updated == 1);
    scene.frame_advance();
    REQUIRE(test2.updated == 2);
    REQUIRE(on_delete == 0);
    scene.frame_advance();
    REQUIRE(test2.updated == 3);
    REQUIRE(on_delete == 0);
    scene.frame_advance();
    REQUIRE(test2.updated == 4);
    // It isn't really specified if the object is deleted instantly or if it
    // waits a frame so we don't check on_delete here
    scene.frame_advance();
    REQUIRE(on_delete == 1);
}

TEST_CASE("Animatable different framerates", "[animation]") {
    auto test1 = TestAnimatable();
    auto test2 = TestAnimatable();
    auto scene1 = TestScene(1, 1, 1, 1, 3);
    auto scene2 = TestScene(1, 1, 1, 1, 6);
    test1.set_fps(3);
    test2.set_fps(6);
    animate(scene1, test1);
    animate(scene2, test2);
    test1.update();
    test2.update();
    test2.update();
    REQUIRE(test1.last_t == test2.last_t);
    test1.update();
    test2.update();
    test2.update();
    REQUIRE(test1.last_t == test2.last_t);
    test1.update();
    test2.update();
    test2.update();
    REQUIRE(test1.last_t == test2.last_t);
}

TEST_CASE("Animating lvalues that are not directly in the scene", "[animation]")
{
    auto test1 = TestObject();
    auto test2 = Group(test1);
    auto scene = TestScene(1, 1, 1, 1, 1);
    scene.add(test2);
    animate(scene, test2[0]).set_color(Color("FF0000"));
    REQUIRE(test1.get_color() == Color("FFFFFF"));
    scene.wait(1);
    REQUIRE(test1.get_color() == Color("FF0000"));
}

TEST_CASE("Animating rvalues", "[animation]")
{
    auto test = TestObject();
    auto scene = TestScene(1, 1, 1, 1, 1);
    scene.add(test);
    animate(scene, Group(test)).set_color(Color("FF0000"));
    REQUIRE(test.get_color() == Color("FFFFFF"));
    scene.wait(1);
    REQUIRE(test.get_color() == Color("FF0000"));
}

TEST_CASE("Animating twice at once", "[animation]") {
    auto test = TestObject();
    auto scene = TestScene(1, 1, 1, 1, 1);
    animate(scene, test);
    REQUIRE_THROWS(animate(scene, test));
}

TEST_CASE("Animating twice in a row", "[animation]") {
    auto test = TestObject();
    auto scene = TestScene(1, 1, 1, 1, 2);
    animate(scene, test);
    scene.wait();
    animate(scene, test);
    scene.wait();
}
