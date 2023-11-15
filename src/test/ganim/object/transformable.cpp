#include <catch2/catch_test_macros.hpp>

#include "test/ganim/ga_equals.hpp"

#include "ganim/object/transformable.hpp"

using namespace ganim;

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
    using namespace pga3;
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
