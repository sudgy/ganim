#include "gex.hpp"

#include "ganim/unicode.hpp"
#include "ganim/unicode_categories.hpp"
#include "ganim/object/text/text_helpers.hpp"

using namespace ganim;

GeX::GeX(const std::vector<std::string>& input) : M_input(input)
{
    M_catcodes.resize(16);
    M_catcodes[0] = {U'\\'};
    M_catcodes[1] = {U'{'};
    M_catcodes[2] = {U'}'};
    M_catcodes[3] = {U'$'};
    M_catcodes[4] = {U'&'};
    M_catcodes[5] = {U'\n'};
    M_catcodes[6] = {U'#'};
    M_catcodes[7] = {U'^'};
    M_catcodes[8] = {U'8'};
    M_catcodes[10] = {U' ', U'\t'};
    M_catcodes[11] = unicode_letter;
    M_catcodes[13] = {U'~'};
    M_catcodes[14] = {U'%'};
}

std::vector<PositionedGlyph> GeX::get_output()
{
    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto codepoints = std::vector<std::u32string>();
    codepoints.resize(M_input.size());
    while (auto token = read_token()) {
        std::visit([&](auto& tok) {
            codepoints[token->group].push_back(tok.codepoint);
        }, token->value);
    }
    return shape_text(font, codepoints);
}

std::optional<GeX::Token> GeX::read_token()
{
    if (M_group_index < ssize(M_input)) {
        auto group = std::string_view(M_input[M_group_index]);
        if (M_string_index < ssize(group)) {
            int byte_size = 0;
            auto codepoint = utf8_get_next_codepoint(
                group.substr(M_string_index),
                &byte_size
            );
            M_string_index += byte_size;
            // TODO: A lot
            return Token(
                CharacterToken(codepoint, get_category_code(codepoint)),
                M_group_index
            );
        }
        else {
            ++M_group_index;
            M_string_index = 0;
            return read_token();
        }
    }
    else return std::nullopt;
}

GeX::CategoryCode GeX::get_category_code(std::uint32_t codepoint)
{
    for (int i = 0; i < 16; ++i) {
        if (i == 12) continue;
        if (M_catcodes[i].contains(codepoint)) return CategoryCode(i);
    }
    return CategoryCode::Other;
}
