#ifndef GANIM_COLOR_HPP
#define GANIM_COLOR_HPP

/** @file
 * @brief The @ref ganim::Color "Color" struct.
 */

#include <cstdint>
#include <ostream>

namespace ganim {

/** @brief Represents a color.
 *
 * Ganim represents colors using four byte values for red, green, blue, and
 * alpha.
 */
struct Color {
    /** @brief Default constructor.
     *
     * The default state of a color is transparent black, i.e. all values being
     * zero.
     */
    Color()=default;
    /** @brief Constructs a color from an HTML hex string.
     *
     * @param html_string The hex string corresponding to this color.  If the
     * string starts with `#`, it is ignored, allowing you to specify colors
     * both with and without the leading `#`.  Then, if the resulting string is
     * six bytes long, it will interpret each pair of bytes as the red, green,
     * and blue components.  If it is eight bytes long, it will interpret each
     * pair of bytes as the red, green, blue and alpha components.
     *
     * All of the string-related overloads allow for implicit conversions by
     * design.
     */
    Color(std::string_view html_string);
    /** @brief Constructs a color from an HTML hex string.
     *
     * See @ref Color(std::string_view) for the exact format of the string.
     */
    Color(const std::string& html_string);
    /** @brief Constructs a color from an HTML hex string.
     *
     * See @ref Color(std::string_view) for the exact format of the string.
     */
    Color(const char* html_string);
    /** @brief Constructs a color directly from the individual byte values.
     *
     * The parameters are `int`s just to make overload resolution work with the
     * floating-point overload.  These values are instantly `static_cast`ed to
     * `std::uint8_t`.
     */
    Color(int r, int g, int b, int a);
    /** @brief Constructs a color from float values.
     *
     * Like in OpenGL, the float values go from 0.0-1.0.  Any other values
     * outside this range will be clamped into this range.  If NaN values are
     * passed, the behavior is undefined.
     */
    Color(double r, double g, double b, double a);
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0;
    constexpr bool operator==(const Color&) const=default;
    constexpr bool operator!=(const Color&) const=default;
};

/** @brief Prints a color to an ostream.
 *
 * If `value.a == 255`, it will output a hex string like "#80FF80", and if not,
 * it will output a hex string like "#80FF8080".
 */
std::ostream& operator<<(std::ostream& os, const Color& value);

}

#endif
