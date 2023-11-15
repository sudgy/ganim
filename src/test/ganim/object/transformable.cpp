#include <catch2/catch_test_macros.hpp>

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
    REQUIRE(test.get_rotor() == 1);
    test = TestTransformable(e12);
    REQUIRE(test.get_rotor() == e12);
    test.apply_rotor(e23);
    REQUIRE(test.get_rotor() == e13);
    REQUIRE(test.last_applied_rotor == e23);
    test.reset();
    REQUIRE(test.get_rotor() == 1);
    REQUIRE(test.last_applied_rotor == -e13);
}
