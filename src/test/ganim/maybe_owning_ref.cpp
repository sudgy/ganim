#include <catch2/catch_test_macros.hpp>

#include "ganim/maybe_owning_ref.hpp"

using namespace ganim;

namespace {
    auto destroyed_count = 0;
    struct TestObject {
        ~TestObject() {++destroyed_count;}
    };
}

TEST_CASE("MaybeOwningRef simple", "[ganim]") {
    destroyed_count = 0;
    {
        auto a = TestObject();
        {
            auto test1 = MaybeOwningRef(a);
            auto test2 = MaybeOwningRef(TestObject());
            REQUIRE(&*test1 == &a);
            REQUIRE(destroyed_count == 1);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2.is_owning());
        }
        REQUIRE(destroyed_count == 2);
    }
    REQUIRE(destroyed_count == 3);
}

TEST_CASE("MaybeOwningRef move constructor", "[ganim]") {
    destroyed_count = 0;
    {
        auto a = TestObject();
        {
            auto b = std::make_unique<MaybeOwningRef<TestObject>>(a);
            auto c = std::make_unique<MaybeOwningRef<TestObject>>(TestObject());
            auto test1 = std::move(*b);
            auto test2 = std::move(*c);
            REQUIRE(&**b == &a);
            REQUIRE(&*test1 == &a);
            REQUIRE(&*test2 == &**c);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2.is_owning());
            REQUIRE(destroyed_count == 1);
            b.reset();
            c.reset();
            REQUIRE(destroyed_count == 1);
        }
        REQUIRE(destroyed_count == 2);
    }
    REQUIRE(destroyed_count == 3);
}

TEST_CASE("MaybeOwningRef move assignment", "[ganim]") {
    destroyed_count = 0;
    {
        auto a = TestObject();
        {
            auto test1 = MaybeOwningRef(a);
            auto test2 = std::make_unique<MaybeOwningRef<TestObject>>(TestObject());
            REQUIRE(destroyed_count == 1);
            auto test3 = MaybeOwningRef(a);
            auto& silence_self_move_warning = test3;
            test3 = std::move(silence_self_move_warning);
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2->is_owning());
            REQUIRE(!test3.is_owning());
            REQUIRE(destroyed_count == 1);
            test3 = std::move(test1);
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2->is_owning());
            REQUIRE(!test3.is_owning());
            REQUIRE(destroyed_count == 1);
            test3 = std::move(*test2);
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &**test2);
            REQUIRE(!test1.is_owning());
            REQUIRE(!test2->is_owning());
            REQUIRE(test3.is_owning());
            test2.reset();
            REQUIRE(destroyed_count == 1);
            test3 = std::move(test1);
            REQUIRE(&*test1 == &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test1.is_owning());
            REQUIRE(!test3.is_owning());
            REQUIRE(destroyed_count == 2);
        }
        REQUIRE(destroyed_count == 2);
    }
    REQUIRE(destroyed_count == 3);
}

TEST_CASE("MaybeOwningRef copy constructor", "[ganim]") {
    destroyed_count = 0;
    {
        auto a = TestObject();
        {
            auto b = std::make_unique<MaybeOwningRef<TestObject>>(a);
            auto c = std::make_unique<MaybeOwningRef<TestObject>>(TestObject());
            auto test1 = *b;
            auto test2 = *c;
            REQUIRE(destroyed_count == 1);
            REQUIRE(!test1.is_owning());
            REQUIRE(!test2.is_owning());
            REQUIRE(&*test1 == &**b);
            REQUIRE(&*test2 == &**c);
            b.reset();
            c.reset();
            REQUIRE(destroyed_count == 2);
            REQUIRE(!test1.is_owning());
            REQUIRE(!test2.is_owning());
        }
        REQUIRE(destroyed_count == 2);
    }
    REQUIRE(destroyed_count == 3);
}

TEST_CASE("MaybeOwningRef copy assignment", "[ganim]") {
    destroyed_count = 0;
    {
        auto a = TestObject();
        {
            auto test1 = MaybeOwningRef(a);
            auto test2 = std::make_unique<MaybeOwningRef<TestObject>>(TestObject());
            REQUIRE(destroyed_count == 1);
            auto test3 = MaybeOwningRef(a);
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2->is_owning());
            REQUIRE(!test3.is_owning());
            test3 = test3;
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test3.is_owning());
            REQUIRE(destroyed_count == 1);
            test3 = test1;
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2->is_owning());
            REQUIRE(!test3.is_owning());
            REQUIRE(destroyed_count == 1);
            test3 = *test2;
            REQUIRE(&*test1 == &a);
            REQUIRE(&**test2 != &a);
            REQUIRE(&*test3 == &**test2);
            REQUIRE(!test1.is_owning());
            REQUIRE(test2->is_owning());
            REQUIRE(!test3.is_owning());
            test2.reset();
            REQUIRE(destroyed_count == 2);
            test3 = test1;
            REQUIRE(&*test1 == &a);
            REQUIRE(&*test3 == &a);
            REQUIRE(!test1.is_owning());
            REQUIRE(!test3.is_owning());
            REQUIRE(destroyed_count == 2);
        }
        REQUIRE(destroyed_count == 2);
    }
    REQUIRE(destroyed_count == 3);
}
