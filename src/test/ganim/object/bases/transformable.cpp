#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/bases/transformable.hpp"
#include "ganim/animation/animation.hpp"

#include "ganim/math.hpp"

using namespace ganim;
using namespace pga3;

namespace {
    class TestTransformable : public Transformable {
        public:
            using Transformable::Transformable;
            pga3::Even last_applied_rotor;
            using Transformable::apply_rotor;
            virtual TestTransformable& apply_rotor(const pga3::Even& rotor)
                override
            {
                Transformable::apply_rotor(rotor);
                last_applied_rotor = rotor;
                return *this;
            }
            std::unique_ptr<TestTransformable> anim_copy() const
                {return std::make_unique<TestTransformable>(*this);}
    };
}

TEST_CASE("Transformable basics", "[object]") {
    auto test = TestTransformable();
    REQUIRE_THAT(test.get_rotor(), GAEquals(1));
    test = TestTransformable(e12);
    REQUIRE_THAT(test.get_rotor(), GAEquals(e12));
    test.apply_rotor(e23);
    REQUIRE_THAT(test.get_rotor(), GAEquals(e13));
    REQUIRE_THAT(test.last_applied_rotor, GAEquals(e23));
    test.reset();
    REQUIRE_THAT(test.get_rotor(), GAEquals(1));
    REQUIRE_THAT(test.last_applied_rotor, GAEquals(-e13));
}

TEST_CASE("Transformable origin", "[object]") {
    auto test = TestTransformable();
    REQUIRE_THAT(test.get_origin(), GAEquals(e123));
    test.apply_rotor(e12);
    REQUIRE_THAT(test.get_origin(), GAEquals(e123));
    test.apply_rotor(1 + e01);
    REQUIRE_THAT(test.get_origin(), GAEquals((2*e1 + e0).dual()));
    test.apply_rotor(e12);
    REQUIRE_THAT(test.get_origin(), GAEquals((-2*e1 + e0).dual()));
}

TEST_CASE("Transformable apply_rotor conversions", "[object]") {
    auto test = TestTransformable();
    const auto& r2 = test.last_applied_rotor;
    test.apply_rotor(vga2::e12);
    REQUIRE_THAT(r2, GAEquals(e12));
    test.apply_rotor(vga3::e23);
    REQUIRE_THAT(r2, GAEquals(e23));
    test.apply_rotor(pga2::e01);
    REQUIRE_THAT(r2, GAEquals(e01));
}

TEST_CASE("Transformable move_to", "[object]") {
    auto test = TestTransformable();
    const auto& r = test.get_rotor();
    const auto& r2 = test.last_applied_rotor;
    test.move_to(3*vga2::e1 + 2*vga2::e2);
    REQUIRE_THAT(~r*e123*r, GAEquals((3*e1 + 2*e2 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((3*e1 + 2*e2 + e0).dual()));
    test.move_to(1*vga3::e1 + 1*vga3::e2 - 1*vga3::e3);
    REQUIRE_THAT(~r*e123*r, GAEquals((e1 + e2 - e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((-2*e1 - e2 - e3 + e0).dual()));
    test.move_to((2*pga2::e1 - pga2::e2 + pga2::e0).dual());
    REQUIRE_THAT(~r*e123*r, GAEquals((2*e1 - e2 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((e1 - 2*e2 + e3 + e0).dual()));
    test.move_to((-e1 + 3*e2 + 2*e3 + e0).dual());
    REQUIRE_THAT(~r*e123*r, GAEquals((-e1 + 3*e2 + 2*e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((-3*e1 + 4*e2 + 2*e3 + e0).dual()));
    test.move_to(pga2::e1 + pga2::e2);
    REQUIRE_THAT(~r*e123*r, GAEquals((e1 + e2 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((2*e1 - 2*e2 - 2*e3 + e0).dual()));
    test.move_to(3*e1 + 2*e2 - e3);
    REQUIRE_THAT(~r*e123*r, GAEquals((3*e1 + 2*e2 - e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((2*e1 + e2 - e3 + e0).dual()));
}

TEST_CASE("Transformable shift", "[object]") {
    auto test = TestTransformable();
    const auto& r = test.get_rotor();
    const auto& r2 = test.last_applied_rotor;
    test.shift(3*vga2::e1 + 2*vga2::e2);
    REQUIRE_THAT(~r*e123*r, GAEquals((3*e1 + 2*e2 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((3*e1 + 2*e2 + e0).dual()));
    test.shift(1*vga3::e1 + 1*vga3::e2 - 1*vga3::e3);
    REQUIRE_THAT(~r*e123*r, GAEquals((4*e1 + 3*e2 - e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((e1 + e2 - e3 + e0).dual()));
    test.shift((2*pga2::e1 - pga2::e2 + pga2::e0).dual());
    REQUIRE_THAT(~r*e123*r, GAEquals((6*e1 + 2*e2 - e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((2*e1 - e2 + e0).dual()));
    test.shift((-e1 + 3*e2 + 2*e3 + e0).dual());
    REQUIRE_THAT(~r*e123*r, GAEquals((5*e1 + 5*e2 + e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((-e1 + 3*e2 + 2*e3 + e0).dual()));
    test.shift(pga2::e1 + pga2::e2);
    REQUIRE_THAT(~r*e123*r, GAEquals((6*e1 + 6*e2 + e3 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((e1 + e2 + e0).dual()));
    test.shift(3*e1 + 2*e2 - e3);
    REQUIRE_THAT(~r*e123*r, GAEquals((9*e1 + 8*e2 + e0).dual()));
    REQUIRE_THAT(~r2*e123*r2, GAEquals((3*e1 + 2*e2 - e3 + e0).dual()));
}

TEST_CASE("Transformable rotate", "[object]") {
    auto test = TestTransformable();
    const auto& r = test.get_rotor();
    const auto& r2 = test.last_applied_rotor;
    const auto up = (e2 + e0).dual();
    test.rotate(τ/4);
    REQUIRE_THAT(~r*up*r, GAEquals((-e1 + e0).dual(), 1e-5));
    REQUIRE_THAT(~r2*up*r2, GAEquals((-e1 + e0).dual(), 1e-5));
    test.rotate(vga2::e1, τ/4);
    REQUIRE_THAT(~r*up*r, GAEquals((e1 - 2*e2 + e0).dual(), 1e-5));
    REQUIRE_THAT(~r2*up*r2, GAEquals((-e2 + e0).dual(), 1e-5));
    test.rotate(pga2::e1, -τ/4);
    REQUIRE_THAT(~r*up*r, GAEquals((-e1 + e0).dual(), 1e-5));
    REQUIRE_THAT(~r2*up*r2, GAEquals((2*e1 + e2 + e0).dual(), 1e-5));
    test.rotate(vga3::e13, τ/4);
    REQUIRE_THAT(~r*up*r, GAEquals((-e3 + e0).dual(), 1e-5));
    REQUIRE_THAT(~r2*up*r2, GAEquals((e2 + e0).dual(), 1e-5));
    test.rotate((pga2::e1 + pga2::e0).dual()*τ/4);
    REQUIRE_THAT(~r*up*r, GAEquals((e1 - e2 - e3 + e0).dual(), 1e-5));
    REQUIRE_THAT(~r2*up*r2, GAEquals((-e2 + e0).dual(), 1e-5));
    test.rotate(2*e13 + 2*e01, τ/8);
    REQUIRE_THAT(~r*up*r, GAEquals((2*e1 - e2 + 2*e3 + e0).dual(), 1e-5));
    REQUIRE_THAT(~r2*up*r2, GAEquals((e1 + e2 + e3 + e0).dual(), 1e-5));
}

TEST_CASE("Transformable animate", "[object]") {
    auto test = TestTransformable();
    const auto& r = test.get_rotor();
    const auto& r2 = test.last_applied_rotor;
    test.set_fps(4);
    auto scene = TestScene(1, 1, 1, 1, 4);
    animate(scene, test, {.rate_function = [](double x){return x*x;}})
        .rotate(e12, τ/2);
    REQUIRE_THAT(r, GAEquals(1));
    test.update();
    REQUIRE_THAT(r, GAEquals(std::cos(τ/64) + std::sin(τ/64)*e12, 1e-5));
    REQUIRE_THAT(r2, GAEquals(std::cos(τ/64) + std::sin(τ/64)*e12, 1e-5));
    test.update();
    REQUIRE_THAT(r, GAEquals(std::cos(τ/16) + std::sin(τ/16)*e12, 1e-5));
    REQUIRE_THAT(r2, GAEquals(std::cos(3*τ/64) + std::sin(3*τ/64)*e12, 1e-5));
    test.update();
    REQUIRE_THAT(r, GAEquals(std::cos(9*τ/64) + std::sin(9*τ/64)*e12, 1e-5));
    REQUIRE_THAT(r2, GAEquals(std::cos(5*τ/64) + std::sin(5*τ/64)*e12, 1e-5));
    test.update();
    REQUIRE_THAT(r, GAEquals(e12, 1e-5));
    REQUIRE_THAT(r2, GAEquals(std::cos(7*τ/64) + std::sin(7*τ/64)*e12, 1e-5));
    test.update();
    REQUIRE_THAT(r, GAEquals(e12, 1e-5));
    REQUIRE_THAT(r2, GAEquals(std::cos(7*τ/64) + std::sin(7*τ/64)*e12, 1e-5));
}

TEST_CASE("Transformable shift negative", "[object]") {
    auto test = TestTransformable();
    test.shift(2*e1);
    REQUIRE_THAT(test.get_rotor(), GAEquals(1 + e01));
}

TEST_CASE("Transformable non-commuting rotors", "[object]") {
    auto test = TestTransformable();
    test.set_fps(2);
    test.shift(e1);
    auto scene = TestScene(1, 1, 1, 1, 2);
    animate(scene, test).rotate(e12, τ/2);
    REQUIRE_THAT(test.get_origin(), GAEquals((e1 + e0).dual(), 1e-5));
    test.update();
    REQUIRE_THAT(test.get_origin(), GAEquals((e2 + e0).dual(), 1e-5));
    test.update();
    REQUIRE_THAT(test.get_origin(), GAEquals((-e1 + e0).dual(), 1e-5));
    test.update();
    REQUIRE_THAT(test.get_origin(), GAEquals((-e1 + e0).dual(), 1e-5));
}

TEST_CASE("Transformable interpolate", "[object]") {
    using namespace pga3;
    auto test1 = TestTransformable();
    auto test2 = TestTransformable();
    auto test3 = TestTransformable();
    test2.shift(e1);
    test3.shift(5*e1);
    test1.interpolate(test2, test3, 0.25);
    REQUIRE_THAT(test1.get_rotor(), GAEquals(1 + e01));
    test1.interpolate(test2, test3, 0.5);
    REQUIRE_THAT(test1.get_rotor(), GAEquals(1 + 1.5*e01));
    test1.interpolate(test2, test3, 0.75);
    REQUIRE_THAT(test1.get_rotor(), GAEquals(1 + 2*e01));
    test1.interpolate(test2, test3, 1);
    REQUIRE_THAT(test1.get_rotor(), GAEquals(1 + 2.5*e01));
}

TEST_CASE("Transformable move_to interpolating", "[object]") {
    using namespace pga3;
    auto test1 = TestTransformable();
    auto test2 = TestTransformable();
    auto test3 = TestTransformable();
    test3.move_to(-e2);
    test1.interpolate(test2, test3, 0.5);
    REQUIRE_THAT(test1.get_rotor(), GAEquals(1 - 0.25*e02));
}
