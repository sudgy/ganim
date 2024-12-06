#include <catch2/catch_test_macros.hpp>
#include "test/ganim/ga_equals.hpp"
#include "test/ganim/scene/test_scene.hpp"
#include "test/ganim/approx_color.hpp"

#include "ganim/object/vector.hpp"

#include "ganim/math.hpp"

using namespace ganim;

TEST_CASE("Vector object construction", "[object]") {
    using namespace pga3;
    auto test1 = Vector(e1);
    auto test2 = Vector(e2);
    auto test3 = Vector(e1 + e2);
    auto test4 = Vector(e1 + e3);
    auto test5 = Vector(e3);
    auto test6 = Vector(e1, 2*e1);
    auto test7 = Vector(e1, e1 + e3);

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
    auto test = Vector(e1);
    test.scale(2);
    REQUIRE_THAT(test.get_start_vga3(), GAEquals(0));
    REQUIRE_THAT(test.get_end_vga3(), GAEquals(2*e1));
    test.scale(2, e1);
    REQUIRE_THAT(test.get_start_vga3(), GAEquals(-e1));
    REQUIRE_THAT(test.get_end_vga3(), GAEquals(3*e1));
}

TEST_CASE("Vector object transforms", "[object]") {
    using namespace pga3;
    auto test = Vector(e1);
    test.rotate(τ/4, e12);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));

    test.rotate(τ/4, e23);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    auto r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + e1, 1e-5));

    test.shift(e1);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0 + e1, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e1 + e3, 1e-5));
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + 2*e1, 1e-5));

    test.rotate(τ/4, e13);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0 + e3, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e1 + e3, 1e-5));
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e0 - e2).dual()*r).undual(), GAEquals(e0 + e2+e3, 1e-5));
}

TEST_CASE("Vector object zero vector", "[object]") {
    using namespace pga3;
    auto test = Vector(0*e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.set_end(e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e1));
    test.set_end(0*e1);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.rotate(τ/4, e12);
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3(), GAEquals(e123));
    test.reset_scale();
    REQUIRE_THAT(test.get_start_pga3(), GAEquals(e123));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 + e2, 1e-5));
}

TEST_CASE("Vector turning around", "[object]") {
    using namespace pga3;
    auto test = Vector(e1);
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
    auto test = Vector(e1);
    test.rotate(τ/2, e13);
    REQUIRE_THAT(test.get_start_pga3().undual(), GAEquals(e0, 1e-5));
    REQUIRE_THAT(test.get_end_pga3().undual(), GAEquals(e0 - e1, 1e-5));
}

TEST_CASE("Vector drawing", "[object]") {
    using namespace pga3;
    auto test = Vector(12*e1, {0.5, 6.2, 2});
    auto scene = TestScene(40, 10, 40, 10, 1);
    test.set_visible(true);
    test.scale(0.5);
    scene.add(test);
    scene.frame_advance();
    test.scale(2);
    scene.frame_advance();
    test.scale(1.5);
    scene.frame_advance();
    const auto white = Color("FFFFFF");
    const auto black = Color("000000");
    REQUIRE(scene.get_pixel(0, 20, 4) == white);
    REQUIRE(scene.get_pixel(0, 21, 4) == white);
    REQUIRE(scene.get_pixel(0, 22, 4) == white);
    REQUIRE(scene.get_pixel(0, 23, 4) == white);
    REQUIRE(scene.get_pixel(0, 24, 4) == white);
    REQUIRE(scene.get_pixel(0, 20, 5) == white);
    REQUIRE(scene.get_pixel(0, 21, 5) == white);
    REQUIRE(scene.get_pixel(0, 22, 5) == white);
    REQUIRE(scene.get_pixel(0, 23, 5) == white);
    REQUIRE(scene.get_pixel(0, 24, 5) == white);
    REQUIRE(scene.get_pixel(0, 23, 3) != black);
    REQUIRE(scene.get_pixel(0, 23, 6) != black);
    REQUIRE(scene.get_pixel(0, 25, 2) == black);
    REQUIRE(scene.get_pixel(0, 25, 7) == black);
    REQUIRE(scene.get_pixel(0, 27, 4) == black);
    REQUIRE(scene.get_pixel(0, 27, 5) == black);

    REQUIRE(scene.get_pixel(1, 20, 4) == white);
    REQUIRE(scene.get_pixel(1, 21, 4) == white);
    REQUIRE(scene.get_pixel(1, 22, 4) == white);
    REQUIRE(scene.get_pixel(1, 23, 4) == white);
    REQUIRE(scene.get_pixel(1, 24, 4) == white);
    REQUIRE(scene.get_pixel(1, 25, 4) == white);
    REQUIRE(scene.get_pixel(1, 26, 4) == white);
    REQUIRE(scene.get_pixel(1, 27, 4) == white);
    REQUIRE(scene.get_pixel(1, 28, 4) == white);
    REQUIRE(scene.get_pixel(1, 20, 5) == white);
    REQUIRE(scene.get_pixel(1, 21, 5) == white);
    REQUIRE(scene.get_pixel(1, 22, 5) == white);
    REQUIRE(scene.get_pixel(1, 23, 5) == white);
    REQUIRE(scene.get_pixel(1, 24, 5) == white);
    REQUIRE(scene.get_pixel(1, 25, 5) == white);
    REQUIRE(scene.get_pixel(1, 26, 5) == white);
    REQUIRE(scene.get_pixel(1, 27, 5) == white);
    REQUIRE(scene.get_pixel(1, 28, 5) == white);
    REQUIRE(scene.get_pixel(1, 26, 3) != black);
    REQUIRE(scene.get_pixel(1, 26, 6) != black);
    REQUIRE(scene.get_pixel(1, 31, 2) == black);
    REQUIRE(scene.get_pixel(1, 31, 7) == black);
    REQUIRE(scene.get_pixel(1, 33, 4) == black);
    REQUIRE(scene.get_pixel(1, 33, 5) == black);

    REQUIRE(scene.get_pixel(2, 20, 4) == white);
    REQUIRE(scene.get_pixel(2, 21, 4) == white);
    REQUIRE(scene.get_pixel(2, 22, 4) == white);
    REQUIRE(scene.get_pixel(2, 23, 4) == white);
    REQUIRE(scene.get_pixel(2, 24, 4) == white);
    REQUIRE(scene.get_pixel(2, 25, 4) == white);
    REQUIRE(scene.get_pixel(2, 26, 4) == white);
    REQUIRE(scene.get_pixel(2, 27, 4) == white);
    REQUIRE(scene.get_pixel(2, 28, 4) == white);
    REQUIRE(scene.get_pixel(2, 29, 4) == white);
    REQUIRE(scene.get_pixel(2, 30, 4) == white);
    REQUIRE(scene.get_pixel(2, 31, 4) == white);
    REQUIRE(scene.get_pixel(2, 32, 4) == white);
    REQUIRE(scene.get_pixel(2, 33, 4) == white);
    REQUIRE(scene.get_pixel(2, 34, 4) == white);
    REQUIRE(scene.get_pixel(2, 20, 5) == white);
    REQUIRE(scene.get_pixel(2, 21, 5) == white);
    REQUIRE(scene.get_pixel(2, 22, 5) == white);
    REQUIRE(scene.get_pixel(2, 23, 5) == white);
    REQUIRE(scene.get_pixel(2, 24, 5) == white);
    REQUIRE(scene.get_pixel(2, 25, 5) == white);
    REQUIRE(scene.get_pixel(2, 26, 5) == white);
    REQUIRE(scene.get_pixel(2, 27, 5) == white);
    REQUIRE(scene.get_pixel(2, 28, 5) == white);
    REQUIRE(scene.get_pixel(2, 29, 5) == white);
    REQUIRE(scene.get_pixel(2, 30, 5) == white);
    REQUIRE(scene.get_pixel(2, 31, 5) == white);
    REQUIRE(scene.get_pixel(2, 32, 5) == white);
    REQUIRE(scene.get_pixel(2, 33, 5) == white);
    REQUIRE(scene.get_pixel(2, 34, 5) == white);
    REQUIRE(scene.get_pixel(2, 32, 3) != black);
    REQUIRE(scene.get_pixel(2, 32, 6) != black);
    REQUIRE(scene.get_pixel(2, 37, 2) == black);
    REQUIRE(scene.get_pixel(2, 37, 7) == black);
    REQUIRE(scene.get_pixel(2, 39, 4) == black);
    REQUIRE(scene.get_pixel(2, 39, 5) == black);
}

TEST_CASE("Vector object 2D/3D", "[object]") {
    auto v1 = Vector(2*vga2::e1, {.thickness = 2});
    auto v2 = Vector(vga2::e1, {.thickness = 2, .three_d = true});
    auto scene = TestScene(4, 4, 4, 4, 1);
    scene.add(v1, v2);
    scene.get_camera().rotate(τ/4, vga3::e23);
    v1.set_visible(true);
    scene.frame_advance();
    v1.set_visible(false);
    v2.set_visible(true);
    scene.frame_advance();
    REQUIRE(scene.get_pixel(0, 2, 1) == ApproxColor("000000"));
    REQUIRE(scene.get_pixel(1, 2, 1) == ApproxColor("FFFFFF"));
}

TEST_CASE("Vector object orientation locking", "[object]") {
    using namespace pga3;
    auto test = Vector(e1);
    test.rotate(τ/4, e23);
    auto r = test.get_rotor();
    REQUIRE_THAT((~r*(e2 + e0).dual()*r).undual(), GAEquals(e2 + e0, 1e-5));
    test.lock_orientation(false);
    test.rotate(τ/4, e23);
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e2 + e0).dual()*r).undual(), GAEquals(e3 + e0, 1e-5));
    test.lock_orientation(true);
    r = test.get_rotor();
    REQUIRE_THAT((~r*(e2 + e0).dual()*r).undual(), GAEquals(e2 + e0, 1e-5));
}

TEST_CASE("Vector bounding box", "[object]") {
    using namespace vga2;
    auto test = Vector(4*e1, {.tip_size = 2});
    auto box = test.get_logical_bounding_box();
    REQUIRE_THAT(pga2_to_vga2(box.get_lower_left()), GAEquals(-e2));
    REQUIRE_THAT(pga2_to_vga2(box.get_upper_right()), GAEquals(4*e1 + e2));
}
