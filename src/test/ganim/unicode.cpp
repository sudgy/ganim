#include <catch2/catch_test_macros.hpp>

#include "ganim/unicode.hpp"

using namespace ganim;

TEST_CASE("utf8_to_codepoints", "[ganim]") {
    auto good_string = "a\xC2\x86\xD7\x90\xE3\x81\x8B\xF0\x90\x85\x82";
    auto bad_byte = "\xFF";
    auto late_bad_byte = "ab\xC2\x86\xFF";
    auto short_two_byte = "\xC2";
    auto short_three_byte1 = "\xE3";
    auto short_three_byte2 = "\xE3\x81";
    auto short_four_byte1 = "\xF0";
    auto short_four_byte2 = "\xF0\x90";
    auto short_four_byte3 = "\xF0\x90\x85";
    auto bad_two_byte_cont = "\xC2\x50";
    auto bad_three_byte_cont1 = "\xE3\x50";
    auto bad_three_byte_cont2 = "\xE3\x81\x50";
    auto bad_four_byte_cont1 = "\xF0\x50";
    auto bad_four_byte_cont2 = "\xF0\x90\x50";
    auto bad_four_byte_cont3 = "\xF0\x90\x85\x50";

    auto gen = utf8_to_codepoints(good_string);
    REQUIRE(gen);
    REQUIRE(gen() == 'a');
    REQUIRE(gen);
    REQUIRE(gen() == 0x86U);
    REQUIRE(gen);
    REQUIRE(gen() == 0x5D0U);
    REQUIRE(gen);
    REQUIRE(gen() == 0x304BU);
    REQUIRE(gen);
    REQUIRE(gen() == 0x10142U);
    REQUIRE(!gen);

    auto run = [](auto s) {utf8_to_codepoints(s)();};
    REQUIRE_THROWS_AS(run(bad_byte), std::invalid_argument);
    auto gen2 = utf8_to_codepoints(late_bad_byte);
    REQUIRE(gen2);
    REQUIRE(gen2() == 'a');
    REQUIRE(gen2);
    REQUIRE(gen2() == 'b');
    REQUIRE(gen2);
    REQUIRE(gen2() == 0x86U);
    REQUIRE_THROWS_AS(gen2(), std::invalid_argument);

    REQUIRE_THROWS_AS(run(short_two_byte), std::invalid_argument);
    REQUIRE_THROWS_AS(run(short_three_byte1), std::invalid_argument);
    REQUIRE_THROWS_AS(run(short_three_byte2), std::invalid_argument);
    REQUIRE_THROWS_AS(run(short_four_byte1), std::invalid_argument);
    REQUIRE_THROWS_AS(run(short_four_byte2), std::invalid_argument);
    REQUIRE_THROWS_AS(run(short_four_byte3), std::invalid_argument);
    REQUIRE_THROWS_AS(run(bad_two_byte_cont), std::invalid_argument);
    REQUIRE_THROWS_AS(run(bad_three_byte_cont1), std::invalid_argument);
    REQUIRE_THROWS_AS(run(bad_three_byte_cont2), std::invalid_argument);
    REQUIRE_THROWS_AS(run(bad_four_byte_cont1), std::invalid_argument);
    REQUIRE_THROWS_AS(run(bad_four_byte_cont2), std::invalid_argument);
    REQUIRE_THROWS_AS(run(bad_four_byte_cont3), std::invalid_argument);
}

TEST_CASE("utf16_to_codepoints", "[ganim]") {
    auto good_string = u"a\x86\x05D0\x304B\xD800\xDD42";
    auto too_small = u"\xD800";
    auto late_too_small = u"a\x86\xD800";
    auto malformed = u"\xD800\x50";
    auto swapped = u"\xDD42\xD800";

    auto gen = utf16_to_codepoints(good_string);
    REQUIRE(gen);
    REQUIRE(gen() == 'a');
    REQUIRE(gen);
    REQUIRE(gen() == 0x86U);
    REQUIRE(gen);
    REQUIRE(gen() == 0x5D0U);
    REQUIRE(gen);
    REQUIRE(gen() == 0x304BU);
    REQUIRE(gen);
    REQUIRE(gen() == 0x10142U);
    REQUIRE(!gen);

    auto run = [](auto s) {utf16_to_codepoints(s)();};
    REQUIRE_THROWS_AS(run(too_small), std::invalid_argument);
    auto gen2 = utf16_to_codepoints(late_too_small);
    REQUIRE(gen2);
    REQUIRE(gen2() == 'a');
    REQUIRE(gen2);
    REQUIRE(gen2() == 0x86U);
    REQUIRE_THROWS_AS(gen2(), std::invalid_argument);
    REQUIRE_THROWS_AS(run(malformed), std::invalid_argument);
    REQUIRE_THROWS_AS(run(swapped), std::invalid_argument);
}

TEST_CASE("UnicodeCodepoint", "[ganim]") {
    // This test assumes a little-endian architecture
    auto ascii_a = 'a';
    auto utf8_a = "a";
    auto utf16_a = u"a";
    auto utf32_a = unsigned('a');

    auto utf8_dagger = "\xC2\x86";
    auto utf16_dagger = u"\x86";
    auto utf32_dagger = 0x86U;

    auto utf8_aleph = "\xD7\x90";
    auto utf16_aleph = u"\x05D0";
    auto utf32_aleph = 0x5D0U;

    auto utf8_ka = "\xE3\x81\x8B";
    auto utf16_ka = u"\x304B";
    auto utf32_ka = 0x304BU;

    auto utf8_one = "\xF0\x90\x85\x82";
    auto utf16_one = u"\xD800\xDD42";
    auto utf32_one = 0x10142U;

    REQUIRE(UnicodeCodepoint(ascii_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint(utf8_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint(utf16_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint(utf32_a).codepoint == utf32_a);

    REQUIRE(UnicodeCodepoint(utf8_dagger).codepoint == utf32_dagger);
    REQUIRE(UnicodeCodepoint(utf16_dagger).codepoint == utf32_dagger);
    REQUIRE(UnicodeCodepoint(utf32_dagger).codepoint == utf32_dagger);

    REQUIRE(UnicodeCodepoint(utf8_aleph).codepoint == utf32_aleph);
    REQUIRE(UnicodeCodepoint(utf16_aleph).codepoint == utf32_aleph);
    REQUIRE(UnicodeCodepoint(utf32_aleph).codepoint == utf32_aleph);

    REQUIRE(UnicodeCodepoint(utf8_ka).codepoint == utf32_ka);
    REQUIRE(UnicodeCodepoint(utf16_ka).codepoint == utf32_ka);
    REQUIRE(UnicodeCodepoint(utf32_ka).codepoint == utf32_ka);

    REQUIRE(UnicodeCodepoint(utf8_one).codepoint == utf32_one);
    REQUIRE(UnicodeCodepoint(utf16_one).codepoint == utf32_one);
    REQUIRE(UnicodeCodepoint(utf32_one).codepoint == utf32_one);

    REQUIRE_THROWS_AS(UnicodeCodepoint(""), std::invalid_argument);
    REQUIRE_THROWS_AS(UnicodeCodepoint(u""), std::invalid_argument);
}
