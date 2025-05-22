#include "gex.hpp"

#include "ganim/unicode.hpp"
#include "ganim/unicode_categories.hpp"
#include "ganim/object/text/text_helpers.hpp"

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

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
    M_catcodes[8] = {U'_'};
    M_catcodes[10] = {U' ', U'\t'};
    M_catcodes[11] = unicode_letter;
    M_catcodes[13] = {U'~'};
    M_catcodes[14] = {U'%'};

    M_macros[U"relax"] = {};
    M_macros[U"{"] = {{CharacterToken(U'{', CategoryCode::Other)}};
    M_macros[U"}"] = {{CharacterToken(U'}', CategoryCode::Other)}};
    M_macros[U"$"] = {{CharacterToken(U'$', CategoryCode::Other)}};
    M_macros[U"&"] = {{CharacterToken(U'&', CategoryCode::Other)}};
    M_macros[U"#"] = {{CharacterToken(U'#', CategoryCode::Other)}};
    M_macros[U"^"] = {{CharacterToken(U'^', CategoryCode::Other)}};
    M_macros[U"_"] = {{CharacterToken(U'_', CategoryCode::Other)}};
    M_macros[U"%"] = {{CharacterToken(U'%', CategoryCode::Other)}};
}

std::vector<PositionedGlyph> GeX::get_output()
{
    auto& font = get_font("fonts/NewCM10-Regular.otf");
    M_output_codepoints.resize(M_input.size());
    while (auto token = read_token()) {
        std::visit(overloaded{
            [&](CharacterToken& tok)
                {process_character_token(tok, token->group);},
            [&](CommandToken& tok)
                {process_command_token(tok, token->group);}
        }, token->value);
    }
    return shape_text(font, M_output_codepoints);
}

void GeX::process_character_token(CharacterToken tok, int group)
{
    M_output_codepoints[group].push_back(tok.codepoint);
}

void GeX::process_command_token(CommandToken tok, int group)
{
    auto it = M_macros.find(tok.command);
    if (it == M_macros.end()) {
        throw GeXError(M_group_index, M_string_index,
            std::format("Undefined control sequence \"{}\"",tok.command_utf8));
    }
    auto& tokens = it->second;
    M_next_tokens.prepend_range(tokens);
    for (int i = 0; i < ssize(tokens); ++i) {
        M_next_tokens[i].group = group;
    }
}

std::optional<std::uint32_t> GeX::read_character()
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
            return codepoint;
        }
        else {
            ++M_group_index;
            M_string_index = 0;
            return read_character();
        }
    }
    else return std::nullopt;
}

std::optional<GeX::Token> GeX::read_token()
{
    if (!M_next_tokens.empty()) {
        auto result = M_next_tokens.front();
        M_next_tokens.pop_front();
        return result;
    }
    return read_character().transform([&](auto codepoint) {
        auto category_code = get_category_code(codepoint);
        if (category_code == CategoryCode::Escape) {
            return read_escape();
        }
        else return Token(
            CharacterToken(codepoint, get_category_code(codepoint)),
            M_group_index
        );
    });
}

GeX::Token GeX::read_escape()
{
    auto group = std::string_view(M_input[M_group_index]);
    auto name = std::u32string();
    auto start_index = M_string_index;
    while (M_string_index < ssize(group)) {
        int byte_size = 0;
        auto codepoint = utf8_get_next_codepoint(
            group.substr(M_string_index),
            &byte_size
        );
        if (get_category_code(codepoint) == CategoryCode::Letter) {
            name += codepoint;
            M_string_index += byte_size;
        }
        else {
            // We are in a single character command
            if (start_index == M_string_index) {
                name += codepoint;
                M_string_index += byte_size;
            }
            break;
        }
    }
    auto name_utf8 = group.substr(start_index, M_string_index - start_index);
    // Skip spaces after name
    while (M_string_index < ssize(group)) {
        int byte_size = 0;
        auto codepoint = utf8_get_next_codepoint(
            group.substr(M_string_index),
            &byte_size
        );
        if (get_category_code(codepoint) == CategoryCode::Spacer) {
            M_string_index += byte_size;
        }
        else break;
    }
    return Token(CommandToken(name, std::string(name_utf8)), M_group_index);
}

GeX::CategoryCode GeX::get_category_code(std::uint32_t codepoint)
{
    for (int i = 0; i < 16; ++i) {
        if (i == 12) continue;
        if (M_catcodes[i].contains(codepoint)) return CategoryCode(i);
    }
    return CategoryCode::Other;
}
