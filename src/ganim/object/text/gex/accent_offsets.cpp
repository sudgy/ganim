#include "accent_offsets.hpp"

using namespace ganim;

double gex::get_accent_offset(std::uint32_t codepoint)
{
    switch (codepoint) {
        case 0x1D462: return 0.05;
        case 0x1D463: return 0.05;
        default: return 0.0;
    }
}
