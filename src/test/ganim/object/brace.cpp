#include <catch2/catch_test_macros.hpp>
#include "test/ganim/scene/test_scene.hpp"

#include "ganim/object/brace.hpp"

#include "ganim/ga/exp.hpp"

using namespace ganim;

TEST_CASE("Brace basics", "[object]") {
    using namespace vga2;
    auto brace = make_brace(-4*e1, 4*e1, {true, 2, 0.5, 2});

    const auto r = ga_exp(e12*τ/8);
    auto points = std::vector{
        -4*e1,
        -3*e1 - e1 * ~r,
        -3*e1 + e2,
        -2*e1 + e2,
        -e1 + 2*e2 + e1 * ~r - 0.5*e1,
        2*e2,
        e1 + 2*e2 - e1 * r + 0.5*e1,
        2*e1 + e2,
        3*e1 + e2,
        3*e1 + e1 * r,
        4*e1
    };
    auto path = make_path(points, false, 0.5);

    auto scene = TestScene(32, 32, 16, 16, 1);
    scene.check_draw_equivalent(brace, path);
}

TEST_CASE("Brace on the right", "[object]") {
    using namespace vga2;
    auto brace = make_brace(-4*e1, 4*e1, {false, 2, 0.5, 2});

    const auto r = ga_exp(e12*τ/8);
    auto points = std::vector{
        -4*e1,
        -3*e1 - e1 * ~r,
        -3*e1 + e2,
        -2*e1 + e2,
        -e1 + 2*e2 + e1 * ~r - 0.5*e1,
        2*e2,
        e1 + 2*e2 - e1 * r + 0.5*e1,
        2*e1 + e2,
        3*e1 + e2,
        3*e1 + e1 * r,
        4*e1
    };
    for (auto& p : points) {
        p = e1 * p * e1;
    }
    auto path = make_path(points, false, 0.5);

    auto scene = TestScene(32, 32, 16, 16, 1);
    scene.check_draw_equivalent(brace, path);
}

TEST_CASE("Brace on the verge of being squished", "[object]") {
    using namespace vga2;
    auto brace = make_brace(-3*e1, 3*e1, {true, 2, 0.5, 2});

    const auto r = ga_exp(e12*τ/8);
    auto points = std::vector{
        -3*e1,
        -2*e1 - e1 * ~r,
        -2*e1 + e2,
        -e1 + 2*e2 + e1 * ~r - 0.5*e1,
        2*e2,
        e1 + 2*e2 - e1 * r + 0.5*e1,
        2*e1 + e2,
        2*e1 + e1 * r,
        3*e1
    };
    auto path = make_path(points, false, 0.5);

    auto scene = TestScene(32, 32, 16, 16, 1);
    scene.check_draw_equivalent(brace, path);
}

TEST_CASE("Squished brace", "[object]") {
    using namespace vga2;
    auto brace = make_brace(-1.5*e1, 1.5*e1, {true, 2, 0.5, 2});

    const auto r = ga_exp(e12*τ/8);
    auto points = std::vector{
        -3*e1,
        -2*e1 - e1 * ~r,
        -2*e1 + e2,
        -e1 + 2*e2 + e1 * ~r - 0.5*e1,
        2*e2,
        e1 + 2*e2 - e1 * r + 0.5*e1,
        2*e1 + e2,
        2*e1 + e1 * r,
        3*e1
    };
    for (auto& p : points) {
        p -= 0.5*p.blade_project<e1>()*e1;
    }
    auto path = make_path(points, false, 0.5);

    auto scene = TestScene(32, 32, 16, 16, 1);
    scene.check_draw_equivalent(brace, path);
}

TEST_CASE("Brace recreate", "[object]") {
    using namespace vga2;
    auto brace = make_brace(-4*e1, 4*e1, {true, 2, 0.5, 2});
    brace->recreate(-4*e1, 4*e1, {false, 2, 0.5, 2});

    const auto r = ga_exp(e12*τ/8);
    auto points = std::vector{
        -4*e1,
        -3*e1 - e1 * ~r,
        -3*e1 + e2,
        -2*e1 + e2,
        -e1 + 2*e2 + e1 * ~r - 0.5*e1,
        2*e2,
        e1 + 2*e2 - e1 * r + 0.5*e1,
        2*e1 + e2,
        3*e1 + e2,
        3*e1 + e1 * r,
        4*e1
    };
    for (auto& p : points) {
        p = e1 * p * e1;
    }
    auto path = make_path(points, false, 0.5);

    auto scene = TestScene(32, 32, 16, 16, 1);
    scene.check_draw_equivalent(brace, path);
}
