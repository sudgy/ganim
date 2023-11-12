#include "color.hpp"

#include <algorithm>
#include <format>
#include <cstring>

using namespace ganim;

Color::Color(std::string_view html_string)
{
    if (html_string[0] == '#') html_string.remove_prefix(1);
    if (html_string.size() != 6 and html_string.size() != 8) {
        throw std::invalid_argument("After the leading '#', The string passed "
                "to the Color constructor must be either six or eight "
                "characters long.");
    }
    // This situation is controlled enough that bringing in something like
    // std::stoi is overkill
    auto convert_char = [](char c) {
        if (c >= '0' and c <= '9') return c - '0';
        if (c >= 'a' and c <= 'f') return c - 'a' + 10;
        if (c >= 'A' and c <= 'F') return c - 'A' + 10;
        else throw std::invalid_argument("Character '" + std::string(c, 1)
                + "' invalid in color hex string");
    };
    auto convert_byte = [&](const char* cs) {
        return convert_char(cs[0]) * 0x10 + convert_char(cs[1]);
    };
    r = convert_byte(html_string.data());
    g = convert_byte(html_string.data() + 2);
    b = convert_byte(html_string.data() + 4);
    if (html_string.size() == 8) a = convert_byte(html_string.data() + 6);
    else {
        a = 255;
    }
}

Color::Color(const std::string& html_string)
    : Color(std::string_view(html_string)) {}

Color::Color(const char* html_string)
    : Color(std::string_view(html_string, std::strlen(html_string))) {}

Color::Color(int r, int g, int b, int a)
    : r(r), g(g), b(b), a(a) {}

Color::Color(double r, double g, double b, double a)
    : r(std::clamp(r * 255, 0.0, 255.0)),
      g(std::clamp(g * 255, 0.0, 255.0)),
      b(std::clamp(b * 255, 0.0, 255.0)),
      a(std::clamp(a * 255, 0.0, 255.0)) {}

std::ostream& ganim::operator<<(std::ostream& os, const Color& value)
{
    if (value.a == 255) {
        os << std::format("#{:02X}{:02X}{:02X}", value.r, value.g, value.b);
    }
    else {
        os << std::format("#{:02X}{:02X}{:02X}{:02X}",
                          value.r, value.g, value.b, value.a);
    }
    return os;
}
