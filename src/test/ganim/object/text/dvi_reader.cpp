#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/dvi_reader.hpp"

#include "ganim/coroutine.hpp"

using namespace ganim;

namespace {
    class TestDVIConsumer : public DVIConsumer {
        public:
            virtual int write_character(
                const DVIFont& font,
                std::uint32_t c,
                int h,
                int v
            ) override
            {
                return *M_coroutine(&font, c, h, v);
            }
            virtual void draw_rect(int, int, int, int) override {}
            virtual void set_magnification(double mag) override
            {
                REQUIRE(mag == 25400000.0 / 473628672.0);
            }
            virtual void process_special(std::string_view) override {}
        private:
            using generator_type = ArgsGenerator<
                int,
                const DVIFont*,
                std::uint32_t,
                int,
                int
            >;
            generator_type M_coroutine = coroutine();
            generator_type coroutine()
            {
                auto& args = co_await generator_type::get_args();
                auto& font = get<0>(args);
                auto& c = get<1>(args);
                auto& h = get<2>(args);
                auto& v = get<3>(args);

                auto starting_h = h;
                auto starting_v = v;

                REQUIRE(c == 'H');
                REQUIRE(font->name == "cmr10");
                REQUIRE(font->mag == 1.0);
                REQUIRE(font->size == 10.0);
                REQUIRE(font->system);
                co_yield 0;
                REQUIRE(c == 'e');
                REQUIRE(h - starting_h == 0);
                REQUIRE(v == starting_v);
                co_yield 1;
                REQUIRE(c == 'l');
                REQUIRE(h - starting_h == 1);
                REQUIRE(v == starting_v);
                co_yield 2;
                REQUIRE(c == 'l');
                REQUIRE(h - starting_h == 3);
                REQUIRE(v == starting_v);
                co_yield 3;
                REQUIRE(c == 'o');
                REQUIRE(h - starting_h == 6);
                REQUIRE(v == starting_v);
                co_yield 4;
                REQUIRE(c == ',');
                REQUIRE(h - starting_h == 10);
                REQUIRE(v == starting_v);
                co_yield 5;
                constexpr auto space_width = 218453;
                REQUIRE(c == 'w');
                REQUIRE(h - starting_h == 15 + space_width);
                REQUIRE(v == starting_v);
                co_yield 6;
                constexpr auto kerning = 18205;
                REQUIRE(c == 'o');
                REQUIRE(h - starting_h == 21 + space_width - kerning);
                REQUIRE(v == starting_v);
                co_yield 7;
                REQUIRE(c == 'r');
                REQUIRE(h - starting_h == 28 + space_width - kerning);
                REQUIRE(v == starting_v);
                co_yield 8;
                REQUIRE(c == 'l');
                REQUIRE(h - starting_h == 36 + space_width - kerning);
                REQUIRE(v == starting_v);
                co_yield 9;
                REQUIRE(c == 'd');
                REQUIRE(h - starting_h == 45 + space_width - kerning);
                REQUIRE(v == starting_v);
                co_yield 10;
                REQUIRE(c == '!');
                REQUIRE(h - starting_h == 55 + space_width - kerning);
                REQUIRE(v == starting_v);
            }
    };
}

TEST_CASE("Simple DVIReader", "[object][text]") {
    auto consumer = TestDVIConsumer();
    read_dvi("test_files/test.dvi", consumer);
}
