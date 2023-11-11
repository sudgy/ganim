#ifndef GANIM_COLOR_H
#define GANIM_COLOR_H

#include <cstdint>

namespace ganim {

struct Color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0;
};

}

#endif
