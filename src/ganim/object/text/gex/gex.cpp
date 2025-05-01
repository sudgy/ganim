#include "gex.hpp"

#include "ganim/unicode.hpp"
#include "ganim/object/text/text_helpers.hpp"

using namespace ganim;

GeX::GeX(const std::vector<std::string>& input) : M_input(input) {}

void GeX::tokenize()
{
    for (auto& string : M_input) {
        auto gen = utf8_to_codepoints(string);
        while (auto codepoint = gen()) {
            M_tokens.emplace_back(*codepoint);
        }
    }
}

std::vector<PositionedGlyph> GeX::get_output()
{
    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto codepoints = std::vector<std::u32string>();
    codepoints.resize(1);
    codepoints[0].reserve(M_tokens.size());
    for (auto codepoint : M_tokens) {
        codepoints[0].push_back(codepoint.codepoint);
    }
    return shape_text(font, codepoints);
}
