#include "math_letter_map.hpp"

namespace {
    std::unordered_map<std::uint32_t, std::uint32_t> make_math_letter_map()
    {
        auto result = std::unordered_map<std::uint32_t, std::uint32_t>{

        };
        for (std::uint32_t c = U'A'; c <= U'Z'; ++c) {
            result[c] = U'𝐴' + (c - U'A');
        }
        for (std::uint32_t c = U'a'; c <= U'z'; ++c) {
            result[c] = U'𝑎' + (c - U'a');
        }
        for (std::uint32_t c = U'α'; c <= U'ω'; ++c) {
            result[c] = U'𝛼' + (c - U'α');
        }
        result[U'*'] = U'∗';
        result[U'-'] = U'−';
        return result;
    }
}

std::unordered_map<std::uint32_t, std::uint32_t>
ganim::gex::math_letter_map = make_math_letter_map();
