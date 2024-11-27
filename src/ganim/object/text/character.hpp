#ifndef GANIM_OBJECT_CHARACTER_HPP
#define GANIM_OBJECT_CHARACTER_HPP

/** @file
 * @brief Contains low-level utilities for drawing characters.
 *
 * It does not contain any objects!  Other objects use this for drawing text.
 */

#include <string>

#include "ganim/unicode.hpp"

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
        float texture_x = 0; ///< The x coordinate in the texture
        float texture_y = 0; ///< The y coordinate in the texture
        float texture_width = 0; ///< The width in the texture
        float texture_height = 0; ///< The height in the texture
        double width = 0; ///< The width of the character, in ganim units
        double height = 0; ///< The height of the character, in ganim units
        double bearing_x = 0; ///< @brief The x coordinate of the left side of
                              ///< the character, in ganim units
        double bearing_y = 0; ///< @brief The y coordinate of the top side of
                              ///< the character, in ganim units
        double x_advance = 0; ///< @brief The x coordinate offset where the next
                              ///< character should be, in ganim units
        double x_advance_em = 0; ///< @brief Same as x_advance, but in em rather
                                 ///< than ganim units
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
    Font& get_font(const std::string& filename);

    /** @brief Get a character.
     *
     * @param font A font previously returned by @ref get_font.
     * @param character The Unicode codepoint of the character.  Many
     * representations are implicitly convertible to this type, see @ref
     * UnicodeCodepoint.
     * @return A @ref Character with the information needed to draw the
     * character passed in.
     */
    Character& get_character(Font& font, UnicodeCodepoint character);
    // TODO: Figure out exactly what this is doing so that you can write
    // documentation
    double get_font_pem(Font& font);
    double get_font_ascender(Font& font);
    double get_font_descender(Font& font);
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
