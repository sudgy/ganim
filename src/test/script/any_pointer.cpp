#include <catch2/catch_test_macros.hpp>

#include "script/any_pointer.hpp"

using namespace ganim;

namespace {
    struct A {};
    struct B : A {};
}

TEST_CASE("any_pointer", "[script]") {
    auto p1 = any_pointer();
    auto p2 = any_pointer(nullptr);
    auto x = 1;
    auto y = 1.0;
    auto p3 = any_pointer(&x);
    auto p4 = any_pointer(&y);
    auto b = B();
    auto& a = static_cast<A&>(b);
    auto p5 = any_pointer(&b);
    auto p6 = any_pointer(&a);

    REQUIRE(p1.get_as<int>() == nullptr);
    REQUIRE(p1.get_as<A>() == nullptr);
    REQUIRE(p1.holds_type<int>() == false);
    REQUIRE(p1.holds_type<A>() == false);
    REQUIRE(p1.get_tag() == 0);

    REQUIRE(p2.get_as<int>() == nullptr);
    REQUIRE(p2.get_as<A>() == nullptr);
    REQUIRE(p2.holds_type<int>() == false);
    REQUIRE(p2.holds_type<A>() == false);
    REQUIRE(p2.get_tag() == 0);

    REQUIRE(p3.get_as<int>() == &x);
    REQUIRE(p3.get_as<A>() == nullptr);
    REQUIRE(p3.holds_type<int>() == true);
    REQUIRE(p3.holds_type<A>() == false);
    REQUIRE(p3.get_tag() == any_pointer::get_tag<int>());

    REQUIRE(p4.get_as<double>() == &y);
    REQUIRE(p4.get_as<A>() == nullptr);
    REQUIRE(p4.holds_type<double>() == true);
    REQUIRE(p4.holds_type<A>() == false);
    REQUIRE(p4.get_tag() == any_pointer::get_tag<double>());

    REQUIRE(p5.get_as<A>() == nullptr);
    REQUIRE(p5.get_as<B>() == &b);
    REQUIRE(p5.holds_type<A>() == false);
    REQUIRE(p5.holds_type<B>() == true);
    REQUIRE(p5.get_tag() == any_pointer::get_tag<B>());

    REQUIRE(p6.get_as<A>() == &a);
    REQUIRE(p6.get_as<B>() == nullptr);
    REQUIRE(p6.holds_type<A>() == true);
    REQUIRE(p6.holds_type<B>() == false);
    REQUIRE(p6.get_tag() == any_pointer::get_tag<A>());

    REQUIRE(p1 == p2);
    REQUIRE(p1 != p3);
    REQUIRE(p1 != p4);
    REQUIRE(p1 != p5);
    REQUIRE(p1 != p6);
    REQUIRE(p5 != p6);

    p4 = &x;
    REQUIRE(p3 == p4);
    *p4.get_as<int>() = 2;
    REQUIRE(*p3.get_as<int>() == 2);

    p6 = p5;
    REQUIRE(p5 == p6);
    REQUIRE(p6.holds_type<B>());
}
