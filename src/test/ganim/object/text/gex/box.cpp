#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/box.hpp"

using namespace ganim;
using namespace ganim::gex;

TEST_CASE("combine_boxes_horizontally", "[object][text][gex]") {
    auto test1 = combine_boxes_horizontally({});
    auto test2 = combine_boxes_horizontally({Box(1, 2, 3)});
    auto test3 = combine_boxes_horizontally({Box(1, 2, 3), Box(1, 2, 3)});
    auto test4 = combine_boxes_horizontally({Box(1, 2, 3), Box(2, 3, 2)});

    REQUIRE(test1.new_box.width == 0);
    REQUIRE(test1.new_box.height == 0);
    REQUIRE(test1.new_box.depth == 0);
    REQUIRE(test1.pluses.size() == 0);

    REQUIRE(test2.new_box.width == 1);
    REQUIRE(test2.new_box.height == 2);
    REQUIRE(test2.new_box.depth == 3);
    REQUIRE(test2.pluses.size() == 1);
    REQUIRE(test2.pluses[0] == std::pair{0.0, 0.0});

    REQUIRE(test3.new_box.width == 2);
    REQUIRE(test3.new_box.height == 2);
    REQUIRE(test3.new_box.depth == 3);
    REQUIRE(test3.pluses.size() == 2);
    REQUIRE(test3.pluses[0] == std::pair{0.0, 0.0});
    REQUIRE(test3.pluses[1] == std::pair{1.0, 0.0});

    REQUIRE(test4.new_box.width == 3);
    REQUIRE(test4.new_box.height == 3);
    REQUIRE(test4.new_box.depth == 3);
    REQUIRE(test4.pluses.size() == 2);
    REQUIRE(test4.pluses[0] == std::pair{0.0, 0.0});
    REQUIRE(test4.pluses[1] == std::pair{1.0, 0.0});
}

TEST_CASE("combine_boxes_vertically", "[object][text][gex]") {
    auto test1 = combine_boxes_vertically({}, 0);
    auto test2 = combine_boxes_vertically({Box(1, 2, 3)}, 0);
    auto test3 = combine_boxes_vertically({Box(1, 2, 3), Box(1, 2, 3)}, 0);
    auto test4 = combine_boxes_vertically({Box(1, 2, 3), Box(1, 2, 3)}, 1);
    auto test5 = combine_boxes_vertically({Box(1, 2, 3), Box(2, 3, 2)}, 0);
    auto test6 = combine_boxes_vertically({Box(1, 2, 3), Box(2, 3, 2)}, 1);

    REQUIRE(test1.new_box.width == 0);
    REQUIRE(test1.new_box.height == 0);
    REQUIRE(test1.new_box.depth == 0);
    REQUIRE(test1.pluses.size() == 0);

    REQUIRE(test2.new_box.width == 1);
    REQUIRE(test2.new_box.height == 2);
    REQUIRE(test2.new_box.depth == 3);
    REQUIRE(test2.pluses.size() == 1);
    REQUIRE(test2.pluses[0] == std::pair{0.0, 0.0});

    REQUIRE(test3.new_box.width == 1);
    REQUIRE(test3.new_box.height == 2);
    REQUIRE(test3.new_box.depth == 8);
    REQUIRE(test3.pluses.size() == 2);
    REQUIRE(test3.pluses[0] == std::pair{0.0, 0.0});
    REQUIRE(test3.pluses[1] == std::pair{0.0, -5.0});

    REQUIRE(test4.new_box.width == 1);
    REQUIRE(test4.new_box.height == 7);
    REQUIRE(test4.new_box.depth == 3);
    REQUIRE(test4.pluses.size() == 2);
    REQUIRE(test4.pluses[0] == std::pair{0.0, 5.0});
    REQUIRE(test4.pluses[1] == std::pair{0.0, 0.0});

    REQUIRE(test5.new_box.width == 2);
    REQUIRE(test5.new_box.height == 2);
    REQUIRE(test5.new_box.depth == 8);
    REQUIRE(test5.pluses.size() == 2);
    REQUIRE(test5.pluses[0] == std::pair{0.0, 0.0});
    REQUIRE(test5.pluses[1] == std::pair{0.0, -6.0});

    REQUIRE(test6.new_box.width == 2);
    REQUIRE(test6.new_box.height == 8);
    REQUIRE(test6.new_box.depth == 2);
    REQUIRE(test6.pluses.size() == 2);
    REQUIRE(test6.pluses[0] == std::pair{0.0, 6.0});
    REQUIRE(test6.pluses[1] == std::pair{0.0, 0.0});
}
