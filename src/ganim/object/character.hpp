#ifndef GANIM_OBJECT_CHARACTER_HPP
#define GANIM_OBJECT_CHARACTER_HPP

/** @file
 * @brief Contains low-level utilities for drawing characters.
 *
 * It does not contain any objects!  Other objects use this for drawing text.
 */

#include <string>

namespace ganim {
    /** @brief A struct representing all of the data needed to draw a particular
     * character.
     *
     * This is a POD struct with a bunch of numerical values giving texture data
     * and character positioning.  The texture data can be used directly in the
     * texture returned by @ref get_text_texture.  For the rest of the values,
     * they correspond to the FreeType values.  However, ganim adds a one pixel
     * buffer to characters, so they won't quite match up with the values that
     * FreeType gives.  If you don't call the FreeType functions yourself though
     * you'll be fine just using these values.
     *
     * If you don't know what the FreeType values represent and just want to
     * know how to draw text, use this process:
     *
     * 1. Pick an origin.  This will be roughly the bottom left side of the
     * text.  Note that this corresponds to the base line, not the beard line.
     * This means that a character like "g" will go below this y coordinate.
     * 2. The bottom left of your character will be at the point
     * `(origin_x + bearing_x, origin_y + bearing_y - height)`, and its width
     * and height will be the width and height members.
     * 3. Add `x_advance` to the origin's x value.
     * 4. Call @ref get_kerning with the current character and the next
     * character, and add the resulting value to the origin's x value.
     * 5. Use this new origin to draw the next character.
     */
    struct Character {
        float texture_x; ///< The x coordinate in the texture
        float texture_y; ///< The y coordinate in the texture
        float texture_width; ///< The width in the texture
        float texture_height; ///< The height in the texture
        double width; ///< The width of the character, in ganim units
        double height; ///< The height of the character, in ganim units
        double bearing_x; ///< @brief The x coordinate of the left side of the
                          ///< character, in ganim units
        double bearing_y; ///< @brief The y coordinate of the top side of the
                          ///< character, in ganim units
        double x_advance; ///< @brief The x coordinate offset where the next
                          ///< character should be, in ganim units
    };
    /** @brief Represents a single unicode codepoint
     *
     * FreeType functions expect unicode codepoints as the inputs to their
     * functions (by default).  This struct is a simple utility to allow for
     * several different kinds of inputs to be used.  You can construct it
     * implicitly from an int (where it's interpreted as a Unicode codepoint), a
     * string (where it's intepreted as a UTF-8 codepoint), or a char (where
     * it's interpreted as an ASCII character).  It also includes functions to
     * explicitly convert from UTF-8 and UTF-16.  The final codepoint can be
     * extracted from the codepoint member.
     *
     * To be honest, you shouldn't have to deal with this type yourself because
     * you can implicitly convert several representations to it and then it's
     * only ever used internally.
     */
    struct UnicodeCodepoint {
        /** @brief Construct the codepoint from an integer representing the
         * codepoint.
         */
        UnicodeCodepoint(int codepoint);
        /** @brief Construct the codepoint from a UTF-8 string.
         *
         * The string will be interpreted as a single UTF-8 codepoint.  Any
         * characters after the first UTF-8 codepoint are ignored.  If the
         * string doesn't start with a valid UTF-8 codepoint, the behavior is
         * undefined.
         */
        UnicodeCodepoint(std::string_view utf8);
        /** @brief Construct the codepoint from an ASCII character.
         *
         * If the input character is not an ASCII character, the behavior is
         * undefined.
         */
        UnicodeCodepoint(char ascii);
        /** @brief Construct the codepoint from a UTF-8 string.
         *
         * The string will be interpreted as a single UTF-8 codepoint.  Any
         * characters after the first UTF-8 codepoint are ignored.  If the
         * string doesn't start with a valid UTF-8 codepoint, the behavior is
         * undefined.
         */
        static UnicodeCodepoint from_utf8(std::string_view utf8);
        /** @brief Construct the codepoint from a UTF-16 string.
         *
         * The string will be interpreted as a single UTF-16 codepoint.  Any
         * characters after the first UTF-16 codepoint are ignored.  If the
         * string doesn't start with a valid UTF-16 codepoint, the behavior is
         * undefined.
         */
        static UnicodeCodepoint from_utf16(std::string_view utf16);
        int codepoint;
    };
    /** @brief A type to keep track of fonts.
     *
     * These aren't actually meant to be used directly by the user, so the type
     * is purposely incomplete except in ganim's implementation.  Use @ref
     * get_font to get a font by reference, and then pass this reference to
     * functions like @ref get_character and @ref get_kerning.  The reference is
     * guaranteed to be valid until `main` finishes.
     */
    class Font;
    /** @brief Get a font.
     *
     * @param filename The filename for the font to use.  It must be a full,
     * valid, file path.  Plain font names like "Arial" are not supported.
     *
     * @return A reference to the new font.
     *
     * @throws std::runtime_error if the font file cannot be opened for any
     * reason.
     */
    Font& get_font(std::string_view filename);

    /** @brief Get a character.
     *
     * @param font A font previously returned by @ref get_font.
     * @param character The Unicode codepoint of the character.  Many
     * representations are implicitly convertible to this type, see @ref
     * UnicodeCodepoint.
     * @return A @ref Character with the information needed to draw the
     * character passed in.
     */
    Character get_character(Font& font, UnicodeCodepoint character);
    /** @brief Get the kerning offset for two characters, in ganim units.
     *
     * @param font A font previously returned by @ref get_font.
     * @param left The Unicode codepoint of the character on the left.  Many
     * representations are implicitly convertible to this type, see @ref
     * UnicodeCodepoint.
     * @param right The Unicode codepoint of the character on the left.  Many
     * representations are implicitly convertible to this type, see @ref
     * UnicodeCodepoint.
     */
    double get_kerning(
        Font& font, UnicodeCodepoint left, UnicodeCodepoint right);
    /** @brief Get the OpenGL texture ID used for text.
     */
    unsigned get_text_texture();
    /** @brief The default size of the texture used for text.
     *
     * The text texture is a square of this size.  If you want to change the
     * size, use @ref set_text_texture_size.
     */
    inline constexpr int GC_default_text_texture_size = 1024;
    /** @brief Set the size for the texture used for text.
     *
     * It will be a square with this size.  Powers of two are recommended.  If
     * you run out of space in your text texture, you may want to call this with
     * a higher value.
     *
     * Note that running this function will invalidate the existing text
     * texture, and everything that relies on it!  If you want to call it, you
     * should call it when there are no text objects present, such as at the
     * beginning of your scene.
     */
    void set_text_texture_size(int size);
}

#endif
