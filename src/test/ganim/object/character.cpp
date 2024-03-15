#include <catch2/catch_test_macros.hpp>

#include "ganim/object/character.hpp"

using namespace ganim;

TEST_CASE("Unicode conversions", "[object]") {
    // This test assumes a little-endian architecture
    auto ascii_a = 'a';
    auto utf8_a = "a";
    auto utf16_a = "a\0";
    auto utf32_a = int('a');

    auto utf8_dagger = "\xC2\x86";
    auto utf16_dagger = "\x86\x00";
    auto utf32_dagger = 0x86;

    auto utf8_aleph = "\xD7\x90";
    auto utf16_aleph = "\xD0\x05";
    auto utf32_aleph = 0x5D0;

    auto utf8_ka = "\xE3\x81\x8B";
    auto utf16_ka = "\x4B\x30";
    auto utf32_ka = 0x304B;

    auto utf8_one = "\xF0\x90\x85\x82";
    auto utf16_one = "\x00\xD8\x42\xDD";
    auto utf32_one = 0x10142;

    REQUIRE(UnicodeCodepoint(ascii_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint(utf8_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint(utf32_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint::from_utf8(utf8_a).codepoint == utf32_a);
    REQUIRE(UnicodeCodepoint::from_utf16(utf16_a).codepoint == utf32_a);

    REQUIRE(UnicodeCodepoint(utf8_dagger).codepoint == utf32_dagger);
    REQUIRE(UnicodeCodepoint(utf32_dagger).codepoint == utf32_dagger);
    REQUIRE(UnicodeCodepoint::from_utf8(utf8_dagger).codepoint == utf32_dagger);
    REQUIRE(UnicodeCodepoint::from_utf16(utf16_dagger).codepoint == utf32_dagger);

    REQUIRE(UnicodeCodepoint(utf8_aleph).codepoint == utf32_aleph);
    REQUIRE(UnicodeCodepoint(utf32_aleph).codepoint == utf32_aleph);
    REQUIRE(UnicodeCodepoint::from_utf8(utf8_aleph).codepoint == utf32_aleph);
    REQUIRE(UnicodeCodepoint::from_utf16(utf16_aleph).codepoint == utf32_aleph);

    REQUIRE(UnicodeCodepoint(utf8_ka).codepoint == utf32_ka);
    REQUIRE(UnicodeCodepoint(utf32_ka).codepoint == utf32_ka);
    REQUIRE(UnicodeCodepoint::from_utf8(utf8_ka).codepoint == utf32_ka);
    REQUIRE(UnicodeCodepoint::from_utf16(utf16_ka).codepoint == utf32_ka);

    REQUIRE(UnicodeCodepoint(utf8_one).codepoint == utf32_one);
    REQUIRE(UnicodeCodepoint(utf32_one).codepoint == utf32_one);
    REQUIRE(UnicodeCodepoint::from_utf8(utf8_one).codepoint == utf32_one);
    REQUIRE(UnicodeCodepoint::from_utf16(utf16_one).codepoint == utf32_one);
}
