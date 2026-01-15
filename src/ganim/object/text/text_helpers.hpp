#ifndef GANIM_OBJECT_TEXT_HELPERS_HPP
#define GANIM_OBJECT_TEXT_HELPERS_HPP

#include <vector>
#include <string>
#include <cstdint>

#include "ganim/unicode.hpp"

namespace ganim {
    struct Glyph {
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
        bool invisible = false;
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
    Font& scale_font(const Font& font, double scale);
    /** @brief Shape a list of strings.
     *
     * The strings will all be concatenated and shaped at once.  The input is a
     * vector to allow for the resulting text to be manipulated in "sections".
     * The `group_index` field of the resulting glyphs will be set to match the
     * index of the glyphs in the original string.  For example, if you call
     * `shape_text({"ab", "c"}), the result will contain three glyphs, with "a"
     * and "b" having a group_index of 0, and "c" having a group_index of 1.
     */
    std::vector<Glyph> shape_text(
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
    std::vector<Glyph> shape_text_manual_groups(
        Font& font,
        const std::vector<std::pair<std::u32string, int>>& text
    );
    std::vector<Glyph> shape_delimiter(
        Font& font,
        std::uint32_t codepoint,
        int group,
        double height
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
    /** @brief Get the font ascender */
    double get_font_ascender(Font& font);
    /** @brief Get the font descender */
    double get_font_descender(Font& font);
    double get_font_em(Font& font);
    double get_font_x_height(Font& font);
    double get_font_quad(Font& font);
    double get_font_num1(Font& font);
    double get_font_num2(Font& font);
    double get_font_num3(Font& font);
    double get_font_denom1(Font& font);
    double get_font_denom2(Font& font);
    double get_font_sup1(Font& font);
    double get_font_sup2(Font& font);
    double get_font_sup3(Font& font);
    double get_font_sub1(Font& font);
    double get_font_sub2(Font& font);
    double get_font_sup_drop(Font& font);
    double get_font_sub_drop(Font& font);
    double get_font_delim1(Font& font);
    double get_font_delim2(Font& font);
    double get_font_axis_height(Font& font);
    double get_font_default_rule_thickness(Font& font);
    double get_font_big_op_spacing1(Font& font);
    double get_font_big_op_spacing2(Font& font);
    double get_font_big_op_spacing3(Font& font);
    double get_font_big_op_spacing4(Font& font);
    double get_font_big_op_spacing5(Font& font);
}

#endif
