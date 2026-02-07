#include "accent_offsets.hpp"

using namespace ganim;

double gex::get_accent_offset(std::uint32_t codepoint)
{
    switch (codepoint) {
        case 0x1D434: return 0.15; // 𝐴
        case 0x1D437: return 0.05; // 𝐷
        case 0x1D43F: return 0.05; // 𝐿
        case 0x1D442: return 0.10; // 𝑂
        case 0x1D443: return 0.10; // 𝑃
        case 0x1D45D: return 0.05; // 𝑝
        case 0x1D462: return 0.05; // 𝑢
        case 0x1D463: return 0.05; // 𝑣
        case 0x1D464: return 0.05; // 𝑤
        default: return 0.0;
    }
}
