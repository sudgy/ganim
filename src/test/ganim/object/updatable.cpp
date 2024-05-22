#include <catch2/catch_test_macros.hpp>

#include "ganim/object/bases/updatable.hpp"

using namespace ganim;

TEST_CASE("Updatable updaters", "[object]") {
    auto test = Updatable();
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
