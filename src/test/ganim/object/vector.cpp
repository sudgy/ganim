#include <catch2/catch_test_macros.hpp>
#include "test/ganim/ga_equals.hpp"
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/vector.hpp"

#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("Vector object construction", "[object]") {
    using namespace pga3;
    auto test1 = ganim::Vector(e1);
    auto test2 = ganim::Vector(e2);
    auto test3 = ganim::Vector(e1 + e2);
    auto test4 = ganim::Vector(e1 + e3);
    auto test5 = ganim::Vector(e3);
    auto test6 = ganim::Vector(e1, 2*e1);
    auto test7 = ganim::Vector(e1, e1 + e3);

    REQUIRE_THAT(test1.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test2.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test3.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test4.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test5.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test6.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test7.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));

    REQUIRE_THAT(test1.get_end_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test2.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));
    REQUIRE_THAT(test3.get_end_pga3().undual(), GAEquals(e0 + e1 + e2, 1e-5));
    REQUIRE_THAT(test4.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));
    REQUIRE_THAT(test5.get_end_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    REQUIRE_THAT(test6.get_end_pga3().undual(), GAEquals(e0 + 2*e1, 1e-5));
    REQUIRE_THAT(test7.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));

    auto r1 = test1.get_rotor();
    auto r2 = test2.get_rotor();
    auto r3 = test3.get_rotor();
    auto r4 = test4.get_rotor();
    auto r5 = test5.get_rotor();
    auto r6 = test6.get_rotor();
    auto r7 = test7.get_rotor();

    auto down = (-e2 + e0).dual();
    REQUIRE_THAT((~r1 * down * r1).undual(), GAEquals(e0 - e2, 1e-5));
    REQUIRE_THAT((~r2 * down * r2).undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT((~r3 * down * r3).undual(),
            GAEquals(e0 + (e1 - e2)/std::sqrt(2), 1e-5));
    REQUIRE_THAT((~r4 * down * r4).undual(), GAEquals(e0 - e2, 1e-5));
    REQUIRE_THAT((~r5 * down * r5).undual(), GAEquals(e0 - e2, 1e-5));
    REQUIRE_THAT((~r6 * down * r6).undual(), GAEquals(e0 + e1 - e2, 1e-5));
    REQUIRE_THAT((~r7 * down * r7).undual(), GAEquals(e0 + e1 - e2, 1e-5));
}

TEST_CASE("Vector object scale", "[object]") {
    using namespace vga3;
    auto test = ganim::Vector(e1);
    test.scale(2);
    REQUIRE_THAT(test.get_start_vga3(), GAEquals(0));
    REQUIRE_THAT(test.get_end_vga3(), GAEquals(2*e1));
    test.scale(e1, 2);
    REQUIRE_THAT(test.get_start_vga3(), GAEquals(-e1));
    REQUIRE_THAT(test.get_end_vga3(), GAEquals(3*e1));
}

TEST_CASE("Vector object transforms", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(e1);
    test.rotate(e12, τ/4);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));

    test.rotate(e23, τ/4);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    auto r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + e1, 1e-5));

    test.shift(e1);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + 2*e1, 1e-5));

    test.rotate(e13, τ/4);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e1 + e3, 1e-5));
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + e2+e3, 1e-5));
}

TEST_CASE("Vector object zero vector", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(0*e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.set_end(e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e1));
    test.set_end(0*e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.rotate(e12, τ/4);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.reset_scale();
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));
}

TEST_CASE("Vector turning around", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(e1);
    test.set_end(-e1);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e1, 1e-5));
    test.set_end(e3);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    test.set_end(-e3);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e3, 1e-5));
}

TEST_CASE("Vector rotating around", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(e1);
    test.rotate(e13, τ/2);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e1, 1e-5));
}

TEST_CASE("Vector drawing", "[object]") {
    using namespace pga3;
    auto test = ganim::Vector(12*e1, {0.5, 6, 2});
    auto scene = TestScene(40, 10, 40, 10, 1);
    test.set_visible(true);
    test.scale(0.5);
    scene.add(test);
    scene.frame_advance();
    test.scale(2);
    scene.frame_advance();
    // There seems to be some kind of ugly rounding that happens with a pure 1.5
    test.scale(1.51);
    scene.frame_advance();
    scene.write_frames_to_file("test");
    const auto white = Color("FFFFFF");
    const auto black = Color("000000");
    for (int x = 0; x < 40; ++x) {
        for (int y = 0; y < 10; ++y) {
            for (int t = 0; t < 3; ++t) {
                INFO("x = " << x << ", y = " << y << ", t = " << t);
                auto color = black;
                if (x >= 20 and y >= 2 and y < 8) {
                    if (y == 4 or y == 5) {
                        if (x < 25) color = white;
                        else if (x < 30 and t == 1) color = white;
                        else if (x < 36 and t == 2) color = white;
                    }
                    if (t == 0) {
                        if (x == 23 and (y == 3 or y == 6)) {
                            color = white;
                        }
                        else if ((x == 23 and y == 2) or
                                 (x == 24 and y == 3) or
                                 (x == 25 and y == 4) or
                                 (x == 25 and y == 5) or
                                 (x == 24 and y == 6) or
                                 (x == 23 and y == 7)) continue;
                    }
                    else if (t == 1) {
                        if ((x == 26 or x == 27) and (y == 3 or y == 6)) {
                            color = white;
                        }
                        else if ((x == 26 and y == 2) or
                                 (x == 27 and y == 2) or
                                 (x == 28 and y == 3) or
                                 (x == 29 and y == 3) or
                                 (x == 30 and y == 4) or
                                 (x == 31 and y == 4) or
                                 (x == 31 and y == 5) or
                                 (x == 30 and y == 5) or
                                 (x == 29 and y == 6) or
                                 (x == 28 and y == 6) or
                                 (x == 27 and y == 7) or
                                 (x == 26 and y == 7)) continue;
                    }
                    else {
                        if ((x == 32 or x == 33) and (y == 3 or y == 6)) {
                            color = white;
                        }
                        else if ((x == 32 and y == 2) or
                                 (x == 33 and y == 2) or
                                 (x == 34 and y == 3) or
                                 (x == 35 and y == 3) or
                                 (x == 36 and y == 4) or
                                 (x == 37 and y == 4) or
                                 (x == 37 and y == 5) or
                                 (x == 36 and y == 5) or
                                 (x == 35 and y == 6) or
                                 (x == 34 and y == 6) or
                                 (x == 33 and y == 7) or
                                 (x == 32 and y == 7)) continue;
                    }
                }
                REQUIRE(scene.get_pixel(t, x, y) == color);
            }
        }
    }
}
