#include "dvi_reader.hpp"

#include <fstream>
#include <vector>
#include <stack>
#include <format>
#include <unordered_map>
#include <iostream>

void ganim::read_dvi(std::filesystem::path filename, DVIConsumer& consumer)
{
    auto dvi = std::ifstream(filename, std::ios::binary);
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
            case 128: // set1
            {
                auto c = read_1();
                stack.back().h += consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 129: // set2
            {
                auto c = read_2();
                stack.back().h += consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 130: // set3
            {
                auto c = read_3();
                stack.back().h += consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 131: // set4
            {
                auto c = read_4();
                stack.back().h += consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 132: // set_rule
            {
                auto a = read_4();
                auto b = read_4();
                if (a > 0 and b > 0) {
                    consumer.draw_rect(stack.back().h, stack.back().v, a, b);
                }
                stack.back().h += b;
                break;
            }
            case 133: // put1
            {
                auto c = read_1();
                consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 134: // put2
            {
                auto c = read_2();
                consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 135: // put3
            {
                auto c = read_3();
                consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 136: // put4
            {
                auto c = read_4();
                consumer.write_character(
                    fonts[f],
                    c,
                    stack.back().h,
                    stack.back().v
                );
                break;
            }
            case 137: // put_rule
            {
                auto a = read_4();
                auto b = read_4();
                if (a > 0 and b > 0) {
                    consumer.draw_rect(stack.back().h, stack.back().v, a, b);
                }
                break;
            }
            case 138: // nop
                break;
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
            case 143: // right1
                stack.back().h += read_1();
                break;
            case 144: // right2
                stack.back().h += read_2();
                break;
            case 145: // right3
                stack.back().h += read_3();
                break;
            case 146: // right4
                stack.back().h += read_4();
                break;
            case 147: // w0
                stack.back().h += stack.back().w;
                break;
            case 148: // w1
                stack.back().w = read_1();
                stack.back().h += stack.back().w;
                break;
            case 149: // w2
                stack.back().w = read_2();
                stack.back().h += stack.back().w;
                break;
            case 150: // w3
                stack.back().w = read_3();
                stack.back().h += stack.back().w;
                break;
            case 151: // w4
                stack.back().w = read_4();
                stack.back().h += stack.back().w;
                break;
            case 152: // x0
                stack.back().h += stack.back().x;
                break;
            case 153: // x1
                stack.back().x = read_1();
                stack.back().h += stack.back().x;
                break;
            case 154: // x2
                stack.back().x = read_2();
                stack.back().h += stack.back().x;
                break;
            case 155: // x3
                stack.back().x = read_3();
                stack.back().h += stack.back().x;
                break;
            case 156: // x4
                stack.back().x = read_4();
                stack.back().h += stack.back().x;
                break;
            case 157: // down1
                stack.back().v += read_1();
                break;
            case 158: // down2
                stack.back().v += read_2();
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
            case 162: // y1
                stack.back().y = read_1();
                stack.back().v += stack.back().y;
                break;
            case 163: // y2
                stack.back().y = read_2();
                stack.back().v += stack.back().y;
                break;
            case 164: // y3
                stack.back().y = read_3();
                stack.back().v += stack.back().y;
                break;
            case 165: // y4
                stack.back().y = read_4();
                stack.back().v += stack.back().y;
                break;
            case 166: // z0
                stack.back().v += stack.back().z;
                break;
            case 167: // z1
                stack.back().z = read_1();
                stack.back().v += stack.back().z;
                break;
            case 168: // z2
                stack.back().z = read_2();
                stack.back().v += stack.back().z;
                break;
            case 169: // z3
                stack.back().z = read_3();
                stack.back().v += stack.back().z;
                break;
            case 170: // z4
                stack.back().z = read_4();
                stack.back().v += stack.back().z;
                break;
            case 235: // fnt1
                f = read_1();
                break;
            case 236: // fnt2
                f = read_2();
                break;
            case 237: // fnt3
                f = read_3();
                break;
            case 238: // fnt4
                f = read_4();
                break;
            case 239: // special1
            {
                auto k = read_1();
                auto str = std::string();
                str.resize(k);
                dvi.read(str.data(), k);
                consumer.process_special(str);
                break;
            }
            case 240: // special2
            {
                auto k = read_2();
                auto str = std::string();
                str.resize(k);
                dvi.read(str.data(), k);
                consumer.process_special(str);
                break;
            }
            case 241: // special3
            {
                auto k = read_3();
                auto str = std::string();
                str.resize(k);
                dvi.read(str.data(), k);
                consumer.process_special(str);
                break;
            }
            case 242: // special4
            {
                auto k = read_4();
                auto str = std::string();
                str.resize(k);
                dvi.read(str.data(), k);
                consumer.process_special(str);
                break;
            }
            case 243: // fnt_def1
            case 244: // fnt_def2
            case 245: // fnt_def3
            case 246: // fnt_def4
            {
                auto k = std::uint32_t();
                if (opcode == 243) k = read_1();
                else if (opcode == 244) k = read_2();
                else if (opcode == 245) k = read_3();
                else k = read_4();
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
                fonts[k] = DVIFont(n, mag, d / (1 << 16), system);
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
