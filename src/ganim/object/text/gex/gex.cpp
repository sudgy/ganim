#include "gex.hpp"

#include "ganim/unicode.hpp"
#include "ganim/object/text/text_helpers.hpp"

using namespace ganim;

GeX::GeX(const std::vector<std::string>& input) : M_input(input) {}

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
                CharacterToken(codepoint, CategoryCode::Other),
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
