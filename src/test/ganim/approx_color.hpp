#ifndef GANIM_TEST_APPROX_COLOR_HPP
#define GANIM_TEST_APPROX_COLOR_HPP

#include "ganim/color.hpp"

struct ApproxColor {
    ApproxColor(ganim::Color color);
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
    bool operator==(const ganim::Color& other) const;
};

std::ostream& operator<<(std::ostream& os, const ApproxColor& value);

#endif
