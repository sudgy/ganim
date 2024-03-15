#include "character.hpp"

#include <cstdint>

using namespace ganim;

UnicodeCodepoint::UnicodeCodepoint(int codepoint) : codepoint(codepoint) {}

UnicodeCodepoint::UnicodeCodepoint(std::string_view utf8)
{
    auto string = reinterpret_cast<const std::uint8_t*>(utf8.data());
    if (string[0] & 0x80) {
        if (string[0] >> 5 == 0b110) {
            codepoint = static_cast<int>(string[1] & 0b00111111) +
                       (static_cast<int>(string[0] & 0b00011111) << 6);
        }
        else if (string[0] >> 4 == 0b1110) {
            codepoint = static_cast<int>(string[2] & 0b00111111) +
                       (static_cast<int>(string[1] & 0b00111111) << 6) +
                       (static_cast<int>(string[0] & 0b00001111) << 12);
        }
        // We're evil and don't check for valid UTF-8 >:)
        else {
            codepoint = static_cast<int>(string[3] & 0b00111111) +
                       (static_cast<int>(string[2] & 0b00111111) << 6) +
                       (static_cast<int>(string[1] & 0b00111111) << 12) +
                       (static_cast<int>(string[0] & 0b00000111) << 18);
        }
    }
    else codepoint = string[0];
}

UnicodeCodepoint::UnicodeCodepoint(char ascii) : codepoint(ascii) {}

UnicodeCodepoint UnicodeCodepoint::from_utf8(std::string_view utf8)
{
    return UnicodeCodepoint(utf8);
}

UnicodeCodepoint UnicodeCodepoint::from_utf16(std::string_view utf16)
{
    auto string = reinterpret_cast<const std::uint16_t*>(utf16.data());
    if (string[0] < 0xD7FF or string[0] >= 0xE000) return string[0];
    auto high = static_cast<std::uint32_t>(string[0] - 0xD800);
    auto low = static_cast<std::uint32_t>(string[1] - 0xDC00);
    return static_cast<int>((high << 10) + low + 0x10000U);
}
