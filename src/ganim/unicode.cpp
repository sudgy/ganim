#include "unicode.hpp"

#include <stdexcept>
#include <format>

using namespace ganim;

Generator<std::uint32_t> ganim::utf8_to_codepoints(std::string_view input)
{
    while (input.size() > 0) {
        auto string = reinterpret_cast<const std::uint8_t*>(input.data());
        if (!(string[0] & 0x80)) {
            co_yield string[0];
            input.remove_prefix(1);
            continue;
        }
        if (input.size() < 2) {
            throw std::invalid_argument(std::format(
                "Expected more bytes in a UTF-8 codepoint.  Offending byte:"
                " {:#x}", string[0]));
        }
        if (string[1] >> 6 != 2) {
            throw std::invalid_argument(std::format(
                "Malformed UTF-8 string found.  Offending bytes: {:#x} "
                "{:#x}", string[0], string[1]));
        }
        if (string[0] >> 5 == 0b110) {
            co_yield static_cast<int>(string[1] & 0b00111111) +
                    (static_cast<int>(string[0] & 0b00011111) << 6);
            input.remove_prefix(2);
            continue;
        }
        if (input.size() < 3) {
            throw std::invalid_argument(std::format(
                "Expected more bytes in a UTF-8 codepoint.  Offending bytes:"
                " {:#x} {:#x}", string[0], string[1]));
        }
        if (string[2] >> 6 != 2) {
            throw std::invalid_argument(std::format(
                "Malformed UTF-8 string found.  Offending bytes: {:#x} "
                "{:#x} {:#x}", string[0], string[1], string[2]));
        }
        if (string[0] >> 4 == 0b1110) {
            co_yield static_cast<int>(string[2] & 0b00111111) +
                    (static_cast<int>(string[1] & 0b00111111) << 6) +
                    (static_cast<int>(string[0] & 0b00001111) << 12);
            input.remove_prefix(3);
            continue;
        }
        if (input.size() < 4) {
            throw std::invalid_argument(std::format(
                "Expected more bytes in a UTF-8 codepoint.  Offending bytes:"
                " {:#x} {:#x} {:#x}", string[0], string[1], string[2]));
        }
        if (string[3] >> 6 != 2) {
            throw std::invalid_argument(std::format(
                "Malformed UTF-8 string found.  Offending bytes: {:#x} {:#x} "
                "{:#x} {:#x}", string[0], string[1], string[2], string[3]));
        }
        if (string[0] >> 3 != 0b11110) {
            throw std::invalid_argument(std::format(
                "Malformed UTF-8 string found.  Offending byte: {:#x}",
                string[0]));
        }
        co_yield static_cast<int>(string[3] & 0b00111111) +
                (static_cast<int>(string[2] & 0b00111111) << 6) +
                (static_cast<int>(string[1] & 0b00111111) << 12) +
                (static_cast<int>(string[0] & 0b00000111) << 18);
        input.remove_prefix(4);
    }
}

Generator<std::uint32_t> ganim::utf16_to_codepoints(std::u16string_view input)
{
    while (input.size() > 0) {
        auto string = reinterpret_cast<const std::uint16_t*>(input.data());
        if (string[0] < 0xD7FF or string[0] >= 0xE000) {
            co_yield string[0];
            input.remove_prefix(1);
            continue;
        }
        if (input.size() == 1) {
            throw std::invalid_argument(std::format(
                "Expected more bytes in a UTF-16 codepoint.  Offending values:"
                " {:#x}", static_cast<std::uint16_t>(input[0])));
        }
        if (string[0] >> 10 != 0b110110 or string[1] >> 10 != 0b110111) {
            throw std::invalid_argument(std::format(
                "Malformed UTF-16 string found.  Offending values: {:#x} {:#x}",
                static_cast<std::uint16_t>(input[0]),
                static_cast<std::uint16_t>(input[1])));
        }
        auto high = static_cast<std::uint32_t>(string[0] - 0xD800);
        auto low = static_cast<std::uint32_t>(string[1] - 0xDC00);
        co_yield static_cast<int>((high << 10) + low + 0x10000U);
        input.remove_prefix(2);
    }
}

UnicodeCodepoint::UnicodeCodepoint(unsigned codepoint) : codepoint(codepoint) {}

UnicodeCodepoint::UnicodeCodepoint(std::string_view utf8)
{
    if (utf8.empty()) throw std::invalid_argument(
            "An empty string was passed to UnicodeCodepoint's constructor.");
    codepoint = *utf8_to_codepoints(utf8)();
}

UnicodeCodepoint::UnicodeCodepoint(std::u16string_view utf16)
{
    if (utf16.empty()) throw std::invalid_argument(
            "An empty string was passed to UnicodeCodepoint's constructor.");
    codepoint = *utf16_to_codepoints(utf16)();
}

UnicodeCodepoint::UnicodeCodepoint(char ascii) : codepoint(ascii) {}
