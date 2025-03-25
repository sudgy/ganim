#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "script/script.hpp"

#include "script/script_exception.hpp"

using namespace ganim;

namespace {
    int created = 0;
    int executed = 0;
    class Test : public CommandFactoryBase<Test> {
        public:
            Test(Script& script)
            {
                (void)script;
                ++created;
            }
            virtual void execute() const override
            {
                ++executed;
            }

            inline static std::string command_name = "test";

        private:
            // This will be very different in the future
            std::string M_string;
    };
}

TEST_CASE("Script basics") {
    auto script1 = Script("test;");
    auto script2 = Script("test; test;");
    REQUIRE(created == 0);
    REQUIRE(executed == 0);
    script1.compile();
    REQUIRE(created == 1);
    REQUIRE(executed == 0);
    script2.compile();
    REQUIRE(created == 3);
    REQUIRE(executed == 0);
    script1.execute();
    REQUIRE(created == 3);
    REQUIRE(executed == 1);
    script2.execute();
    REQUIRE(created == 3);
    REQUIRE(executed == 3);
}

TEST_CASE("Script error handling", "[script]") {
    // Just check that an empty script works
    auto script = Script("");
    script.compile();
    script.execute();
    REQUIRE_THROWS_WITH(script.consume_token(),
            get_script_exception_message(-1, -1, "Expected token"));

    auto script2 = Script("  foobar");
    REQUIRE_THROWS_WITH(script2.compile(),
            get_script_exception_message(0, 2, "Unknown command \"foobar\""));

    auto script3 = Script("test");
    REQUIRE_THROWS_WITH(script3.compile(),
            get_script_exception_message(-1, -1, "Expected semicolon"));

    auto script4 = Script("test\ntest");
    REQUIRE_THROWS_WITH(script4.compile(),
            get_script_exception_message(1, 0, "Expected semicolon"));

    auto script5 = Script("test;\ntest");
    REQUIRE_THROWS_WITH(script5.compile(),
            get_script_exception_message(-1, -1, "Expected semicolon"));
}
