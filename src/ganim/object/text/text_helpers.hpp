#ifndef GANIM_OBJECT_TEXT_HELPERS_HPP
#define GANIM_OBJECT_TEXT_HELPERS_HPP

#include <vector>
#include <string>
#include <cstdint>

#include "ganim/unicode.hpp"

namespace ganim {
    /** @brief A struct representing all of the data needed to draw a particular
     * glyph.
     *
     * This is a POD struct with a bunch of numerical values giving texture data
     * and glyph positioning.  The texture data can be used directly in the
     * texture returned by @ref get_text_texture.  For the rest of the values,
     * they correspond to the FreeType values.  However, ganim adds a one pixel
     * buffer to glyphs, so they won't quite match up with the values that
     * FreeType gives.  If you don't call the FreeType functions yourself though
     * you'll be fine just using these values.
     *
     * Note that this struct does not contain data about where on the screen to
     * position the glyph.  It only tells you where to draw a glyph relative to
     * a particular cursor position.  If you don't know what the FreeType values
     * represent and just want to know how to draw text, use this process:
     *
     * 1. Pick an origin.  This will be roughly the bottom left side of the
     * glyph.  Note that this corresponds to the base line, not the beard line.
     * This means that a glyphs like "g" will go below this y coordinate.
     * 2. The bottom left of your glyphs will be at the point `(origin_x +
     * bearing_x, origin_y + bearing_y - height)`, and its width and height will
     * be the width and height members.
     */
    struct Glyph {
        float texture_x = 0; ///< The x coordinate in the texture
        float texture_y = 0; ///< The y coordinate in the texture
        float texture_width = 0; ///< The width in the texture
        float texture_height = 0; ///< The height in the texture
        double width = 0; ///< The width of the glyph, in ganim units
        double height = 0; ///< The height of the glyph, in ganim units
        double bearing_x = 0; ///< @brief The x coordinate of the left side of
                              ///< the glyph, in ganim units
        double bearing_y = 0; ///< @brief The y coordinate of the top side of
                              ///< the glyph, in ganim units
    };
    struct PositionedGlyph {
        double x_pos = 0;
        double y_pos = 0;
        double draw_x = 0;
        double draw_y = 0;
        double width = 0;
        double height = 0;
        double y_min = 0;
        double y_max = 0;
        float texture_x = 0; ///< The x coordinate in the texture
        float texture_y = 0; ///< The y coordinate in the texture
        float texture_width = 0; ///< The width in the texture
        float texture_height = 0; ///< The height in the texture
        int group_index = -1;
    };
    /** @brief A type to keep track of fonts.
     *
     * These aren't actually meant to be used directly by the user, so the type
     * is purposely incomplete except in ganim's implementation.  Use @ref
     * get_font to get a font by reference, and then pass this reference to
     * other functions.  The reference is guaranteed to be valid until `main`
     * finishes.
     */
    class Font;
    using glyph_t = std::uint32_t;
    /** @brief Get a font.
     *
     * @param filename The filename for the font to use.  It must be a full,
     * valid, file path.  Plain font names like "Arial" are not supported.
     * @param pixel_size The size, in pixels, of the letters internally.
     * Increasing it produces better quality (up to a point) at the cost of
     * increased GPU usage, possibly filling up the text texture.  You probably
     * shouldn't ever increase it from the default of 128, but you may want to
     * decrease it if you find yourself filling up the text texture.
     *
     * @return A reference to the new font.
     *
     * @throws std::runtime_error if the font file cannot be opened for any
     * reason.
     */
    Font& get_font(const std::string& filename, int pixel_size = 128);
    /** @brief Get a glyph.
     *
     * You probably don't need to call this function, use @ref shape_text
     * instead.
     *
     * @param font A font previously returned by @ref get_font.
     * @param glyph_index The glyph index in the font of the glyph you want.
     * @return A @ref Glyph with the information needed to draw the glyph passed
     * in.
     */
    Glyph& get_glyph(Font& font, glyph_t glyph_index);
    Glyph& get_glyph_by_codepoint(Font& font, glyph_t glyph_index);
    double get_font_em(Font& font);
    /** @brief Get the font ascender */
    double get_font_ascender(Font& font);
    /** @brief Get the font descender */
    double get_font_descender(Font& font);
    /** @brief Shape a list of strings.
     *
     * The strings will all be concatenated and shaped at once.  The input is a
     * vector to allow for the resulting text to be manipulated in "sections".
     * The `group_index` field of the resulting glyphs will be set to match the
     * index of the glyphs in the original string.  For example, if you call
     * `shape_text({"ab", "c"}), the result will contain three glyphs, with "a"
     * and "b" having a group_index of 0, and "c" having a group_index of 1.
     */
    std::vector<PositionedGlyph> shape_text(
        Font& font,
        const std::vector<std::u32string>& text
    );
    /** @brief Shape a list of strings with grouping done manually.
     *
     * This function is similar to @ref shape_text(Font&, const
     * std::vector<std::u32string>&) except that the group each string is in is
     * passed manually into the function.  Each element of the input vector is a
     * pair of the string and the input group.
     */
    std::vector<PositionedGlyph> shape_text_manual_groups(
        Font& font,
        const std::vector<std::pair<std::u32string, int>>& text
    );
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
