#include <catch2/catch_test_macros.hpp>

#include "ganim/object/group.hpp"

using namespace ganim;

TEST_CASE("Group adding", "[object]") {
    auto leaf12 = std::array{
        Object(),
        Object()
    };
    auto& leaf1 = leaf12[0];
    auto& leaf2 = leaf12[1];
    auto leaf3 = Object();
    auto leaf4 = Object();
    auto group1 = Group();
    auto group2 = Group();
    group1.add(leaf12);
    group2.add(group1, leaf3);
    group2.add(leaf4);
    REQUIRE(group1.size() == 2);
    REQUIRE(group2.size() == 3);
    REQUIRE(&group1[0] == &leaf1);
    REQUIRE(&group1[1] == &leaf2);
    REQUIRE(&group2[0] == &group1);
    REQUIRE(&group2[1] == &leaf3);
    REQUIRE(&group2[2] == &leaf4);
    int i = 0;
    for (auto obj : group1) {
        if (i == 0) REQUIRE(obj == &leaf1);
        else if (i == 1) REQUIRE(obj == &leaf2);
        else REQUIRE(false);
        ++i;
    }
    i = 0;
    for (auto obj : group2) {
        if (i == 0) REQUIRE(obj == &group1);
        else if (i == 1) REQUIRE(obj == &leaf3);
        else if (i == 2) REQUIRE(obj == &leaf4);
        else REQUIRE(false);
        ++i;
    }
}
