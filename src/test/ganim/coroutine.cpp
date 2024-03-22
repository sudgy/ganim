#include <catch2/catch_test_macros.hpp>

#include "ganim/coroutine.hpp"

using namespace ganim;

namespace {
    int test_value = 0;
}

TEST_CASE("Simple coroutine", "[ganim]") {
    test_value = 0;
    auto co = []() -> Coroutine {
        REQUIRE(test_value == 0);
        co_await std::suspend_always();
        REQUIRE(test_value == 1);
        co_await std::suspend_always();
        REQUIRE(test_value == 2);
    }();
    co();
    test_value = 1;
    co();
    test_value = 2;
    co();
    test_value = 3;
    co();
    co();
}

TEST_CASE("Empty coroutine", "[ganim]") {
    auto co = []() -> Coroutine{
        co_return;
    }();
    co();
    co();
}

TEST_CASE("Coroutine with arguments", "[ganim]") {
    auto co = []() -> ArgsCoroutine<int>{
        auto& args = co_await ArgsCoroutine<int>::get_args();
        auto& arg = get<0>(args);
        REQUIRE(arg == 1);
        co_await std::suspend_always();
        REQUIRE(arg == 2);
        co_await std::suspend_always();
        REQUIRE(arg == 3);
    }();
    co(1);
    co(2);
    co(3);
    co(4);
    co(5);
}

TEST_CASE("Simple Generator", "[ganim]") {
    auto gen = []() -> Generator<int>{
        co_yield 1;
        co_yield 2;
        co_yield 3;
    }();
    REQUIRE(gen() == 1);
    REQUIRE(gen() == 2);
    REQUIRE(gen() == 3);
    REQUIRE(!gen());
    REQUIRE(!gen());
}

TEST_CASE("Empty Generator", "[ganim]") {
    auto gen = []() -> Generator<int>{
        co_return;
    }();
    REQUIRE(!gen());
    REQUIRE(!gen());
}

TEST_CASE("Generator arguments", "[ganim]") {
    auto make_gen = []() -> ArgsGenerator<int, int>{
        auto& args = co_await ArgsGenerator<int, int>::get_args();
        auto& arg = get<0>(args);
        while (arg != 0) co_yield arg;
    };
    auto gen1 = make_gen();
    REQUIRE(gen1(1) == 1);
    REQUIRE(gen1(2) == 2);
    REQUIRE(gen1(3) == 3);
    REQUIRE(!gen1(0));
    REQUIRE(!gen1(1));
    auto gen2 = make_gen();
    REQUIRE(!gen2(0));
    REQUIRE(!gen2(1));
}
