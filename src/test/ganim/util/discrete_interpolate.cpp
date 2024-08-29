#include <catch2/catch_test_macros.hpp>

#include "ganim/util/discrete_interpolate.hpp"

using namespace ganim;

TEST_CASE("discrete_interpolate equal size", "[util]") {
    auto test = discrete_interpolate(5, 5);
    REQUIRE(test.size() == 5);
    REQUIRE(test[0].size() == 1);
    REQUIRE(test[1].size() == 1);
    REQUIRE(test[2].size() == 1);
    REQUIRE(test[3].size() == 1);
    REQUIRE(test[4].size() == 1);
    REQUIRE(test[0][0] == 0);
    REQUIRE(test[1][0] == 1);
    REQUIRE(test[2][0] == 2);
    REQUIRE(test[3][0] == 3);
    REQUIRE(test[4][0] == 4);
}

TEST_CASE("discrete_interpolate small sizes", "[util]") {
    REQUIRE_THROWS(discrete_interpolate(0, 1));
    REQUIRE_THROWS(discrete_interpolate(1, 0));
    auto test1 = discrete_interpolate(1, 1);
    auto test2 = discrete_interpolate(1, 3);
    auto test3 = discrete_interpolate(3, 1);

    REQUIRE(test1.size() == 1);
    REQUIRE(test1[0].size() == 1);
    REQUIRE(test1[0][0] == 0);

    REQUIRE(test2.size() == 1);
    REQUIRE(test2[0].size() == 3);
    REQUIRE(test2[0][0] == 0);
    REQUIRE(test2[0][1] == 1);
    REQUIRE(test2[0][2] == 2);

    REQUIRE(test3.size() == 3);
    REQUIRE(test3[0].size() == 1);
    REQUIRE(test3[1].size() == 1);
    REQUIRE(test3[2].size() == 1);
    REQUIRE(test3[0][0] == 0);
    REQUIRE(test3[1][0] == 0);
    REQUIRE(test3[2][0] == 0);
}

TEST_CASE("discrete_interpolate complicated cases", "[util]") {
    // I don't care exactly what algorithm discrete_interpolate follows as long
    // as it satisfies these conditions
    auto check = [](int from_size, int to_size) {
        const auto test = discrete_interpolate(from_size, to_size);
        REQUIRE(ssize(test) == from_size);
        auto size1 = -1;
        auto size2 = -1;
        auto val = 0;
        auto consecutive_size = -1;
        auto last_size = -1;
        for (auto& v : test) {
            auto this_size = ssize(v);
            REQUIRE(this_size > 0);

            // Only one of the sizes can be consecutive, e.g. the sizes
            // {1, 2, 1, 1, 2, 1} are valid but {1, 1, 2, 2, 1, 1} are not
            if (last_size != -1) {
                if (this_size == last_size) {
                    if (consecutive_size == -1) consecutive_size = this_size;
                    REQUIRE(consecutive_size == this_size);
                }
            }
            last_size = this_size;

            // Check that the results are in order
            for (auto this_val : v) {
                REQUIRE((this_val == val or this_val == val + 1));
                if (this_val == val + 1) ++val;
                REQUIRE(this_val < to_size);
            }

            // There must be a maximum of two sizes in the result, this will
            // fire if there's more than two
            if (size1 == -1) size1 = this_size;
            else if (size1 != this_size) {
                if (size2 == -1) size2 = this_size;
                else REQUIRE(this_size == size2);
            }
        }
        REQUIRE(val == to_size - 1);
        if (size2 != -1) REQUIRE(std::abs(size1 - size2) == 1);
    };
    check(2, 3);
    check(2, 4);
    check(4, 6);
    check(5, 8);
    check(5, 23);
    check(5, 25);
    check(3, 2);
    check(4, 2);
    check(6, 4);
    check(8, 5);
    check(23, 5);
    check(25, 5);
}
