#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"

#include "ganim/object/transformable.hpp"

using namespace ganim;
using namespace pga3;

namespace {
    class TestTransformable : public Transformable {
        public:
            using Transformable::Transformable;
            pga3::Even last_applied_rotor;
        private:
            virtual void on_apply_rotor(const pga3::Even& rotor) override
            {
                last_applied_rotor = rotor;
            }
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
