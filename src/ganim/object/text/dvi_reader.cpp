#include "dvi_reader.hpp"

#include <fstream>
#include <filesystem>
#include <vector>
#include <stack>
#include <format>
#include <unordered_map>
#include <iostream>

void ganim::read_dvi(std::string_view filename, DVIConsumer& consumer)
{
    auto dvi = std::ifstream(std::filesystem::path(filename), std::ios::binary);
    auto f = 0;
    struct positioning {
        std::int32_t h = 0;
        std::int32_t v = 0;
        std::int32_t w = 0;
        std::int32_t x = 0;
        std::int32_t y = 0;
        std::int32_t z = 0;
    };
    auto stack = std::vector<positioning>();
    auto fonts = std::unordered_map<int, DVIFont>();
    auto global_mag = 0.0;
    auto read_1 = [&]{
        return (unsigned char)dvi.get();
    };
    auto read_2 = [&]{
        auto byte1 = static_cast<std::uint16_t>(dvi.get());
        auto byte2 = static_cast<std::uint16_t>(dvi.get());
        auto un = (byte1 << (std::uint16_t)8U) + byte2;
        return static_cast<std::int16_t>(un);
    };
    auto read_3 = [&]{
        auto byte1 = static_cast<unsigned>(dvi.get());
        auto byte2 = static_cast<unsigned>(dvi.get());
        auto byte3 = static_cast<unsigned>(dvi.get());
        auto un = (byte1 << 16U) + (byte2 << 8U) + byte3;
        if (un < 0x800000U) return static_cast<std::int32_t>(un);
        else return static_cast<std::int32_t>(un + 0xFF000000U);
    };
    auto read_4 = [&]{
        auto byte1 = static_cast<unsigned>(dvi.get());
        auto byte2 = static_cast<unsigned>(dvi.get());
        auto byte3 = static_cast<unsigned>(dvi.get());
        auto byte4 = static_cast<unsigned>(dvi.get());
        auto un = (byte1 << 24U) + (byte2 << 16U) + (byte3 << 8U) + byte4;
        return static_cast<std::int32_t>(un);
    };
    while (dvi) {
        auto opcode = read_1();
        if (opcode <= 127) {
            stack.back().h += consumer.write_character(
                fonts[f],
                opcode,
                stack.back().h,
                stack.back().v
            );
            continue;
        }
        if (opcode >= 171 and opcode <= 234) {
            f = opcode - 171;
            continue;
        }
        switch (opcode) {
            case 139: // bop
            {
                stack.clear();
                stack.emplace_back(0, 0, 0, 0, 0, 0);
                for (int i = 0; i < 11; ++i) {
                    read_4(); // Ignore page numbers and previous bop
                }
                break;
            }
            case 140: // eop
                break;
            case 141: // push
                stack.push_back(stack.back());
                break;
            case 142: // pop
                stack.pop_back();
                break;
            case 144: // right2
                stack.back().h += read_2();
                break;
            case 145: // right3
                stack.back().h += read_3();
                break;
            case 159: // down3
                stack.back().v += read_3();
                break;
            case 160: // down4
                stack.back().v += read_4();
                break;
            case 161: // y0
                stack.back().v += stack.back().y;
                break;
            case 165: // y4
                stack.back().y = read_4();
                stack.back().v += stack.back().y;
                break;
            case 239: // special1
            {
                auto k = read_1();
                dvi.seekg(k, std::ios_base::cur); // nop for now
                break;
            }
            case 243: // fnt_def1
            {
                auto k = read_1();
                if (fonts.find(k) != fonts.end()) break;
                read_4(); // Ignore checksum, not sure how to validate it
                auto s = read_4();
                auto d = read_4();
                auto mag = global_mag * s / (1000.0 * d);
                auto a = read_1();
                auto l = read_1();
                auto system = a == 0;
                auto n = std::string();
                n.resize(a + l);
                dvi.read(n.data(), a + l);
                fonts[k] = DVIFont(n, mag, system);
                break;
            }
            case 247: // Preamble
            {
                auto i = dvi.get();
                if (i != 2) {
                    throw std::runtime_error(std::format(
                        "Unknown DVI format {}", i));
                }
                auto num = read_4();
                auto den = read_4();
                global_mag = read_4();
                std::cout << global_mag << ' ' << num << ' ' << den << '\n';
                consumer.set_magnification(global_mag * num / (1000.0 * den));
                auto k = read_1();
                dvi.seekg(k, std::ios_base::cur);
                break;
            }
            case 248: // Postamble
            {
                return;
            }
            default:
                throw std::runtime_error(std::format(
                        "Unknown DVI opcode {}", static_cast<int>(opcode)));
        }
    }
}
