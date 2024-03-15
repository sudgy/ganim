#ifndef GANIM_UNICODE_HPP
#define GANIM_UNICODE_HPP

#include <cstdint>
#include <string>

#include "generator.hpp"

namespace ganim {
    Generator<std::uint32_t> utf8_to_codepoints(std::string_view input);
    Generator<std::uint32_t> utf16_to_codepoints(std::u16string_view input);

    /** @brief Represents a single unicode codepoint
     *
     * This struct is a simple utility to allow for several different kinds of
     * inputs to be used for a function that wants a Unicode codepoint.  You can
     * construct it implicitly from an int (where it's interpreted as a Unicode
     * codepoint), a string (where it's intepreted as a UTF-8 codepoint), a char
     * (where it's interpreted as an ASCII character), or a u16string_view
     * (where it's interpreted as a UTF-16 codepoint).  The final codepoint can
     * be extracted from the codepoint member.
     *
     * To be honest, you shouldn't have to deal with this type yourself because
     * you can implicitly convert several representations to it and then it's
     * only ever used internally.
     */
    struct UnicodeCodepoint {
        /** @brief Construct the codepoint from an integer representing the
         * codepoint.
         */
        UnicodeCodepoint(unsigned codepoint);
        /** @brief Construct the codepoint from a UTF-8 string.
         *
         * The string will be interpreted as a single UTF-8 codepoint.  Any
         * characters after the first UTF-8 codepoint are ignored.  If the
         * string doesn't start with a valid UTF-8 codepoint, it will throw
         * `std::invalid_argument`.
         */
        UnicodeCodepoint(std::string_view utf8);
        /** @brief Construct the codepoint from a UTF-16 string.
         *
         * The string will be interpreted as a single UTF-16 codepoint.  Any
         * characters after the first UTF-16 codepoint are ignored.  If the
         * string doesn't start with a valid UTF-16 codepoint, it will throw
         * `std::invalid_argument`.
         */
        UnicodeCodepoint(std::u16string_view utf16);
        /** @brief Construct the codepoint from an ASCII character.
         *
         * If the input character is not an ASCII character, the behavior is
         * undefined.
         */
        UnicodeCodepoint(char ascii);
        unsigned codepoint;
    };
}

#endif
