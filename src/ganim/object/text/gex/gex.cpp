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
    M_macros[U"{"] = {{}, {{CharacterToken(U'{', CategoryCode::Other)}}};
    M_macros[U"}"] = {{}, {{CharacterToken(U'}', CategoryCode::Other)}}};
    M_macros[U"$"] = {{}, {{CharacterToken(U'$', CategoryCode::Other)}}};
    M_macros[U"&"] = {{}, {{CharacterToken(U'&', CategoryCode::Other)}}};
    M_macros[U"#"] = {{}, {{CharacterToken(U'#', CategoryCode::Other)}}};
    M_macros[U"^"] = {{}, {{CharacterToken(U'^', CategoryCode::Other)}}};
    M_macros[U"_"] = {{}, {{CharacterToken(U'_', CategoryCode::Other)}}};
    M_macros[U"%"] = {{}, {{CharacterToken(U'%', CategoryCode::Other)}}};

    // Special commands, when an empty token list is found it will try to match
    // up to one of these
    M_macros[U"def"] = {};
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
            std::format("Undefined control sequence \"{}\"", tok.command_utf8));
    }
    auto& macro = it->second;
    int delim_index = 0;
    bool last_space = false;
    while (delim_index < ssize(macro.delimiters)) {
        auto token = read_token();
        if (!token) {
            throw GeXError(M_group_index, M_string_index,
                std::format("Input ended while processing macro \"{}\"",
                    tok.command_utf8));
        }
        if (auto tok = std::get_if<CharacterToken>(&token->value)) {
            if (tok->catcode == CategoryCode::Spacer) {
                if (last_space) continue;
                last_space = true;
                tok->codepoint = U' ';
            }
        }
        if (token->value != macro.delimiters[delim_index].value) {
            throw GeXError(M_group_index, M_string_index,
                std::format("Use of \\{} does not match its definition",
                    tok.command_utf8));
        }
        ++delim_index;
    }
    if (macro.replacement_text.empty()) {
        if (process_built_in(tok.command, group)) return;
    }
    M_next_tokens.prepend_range(macro.replacement_text);
    for (int i = 0; i < ssize(macro.replacement_text); ++i) {
        M_next_tokens[i].group = group;
    }
}

bool GeX::process_built_in(const std::u32string& command, int group)
{
    // I don't know if I'll ever need this but I'll keep it as a parameter just
    // in case
    (void)group;
    if (command == U"def") {
        process_definition();
        return true;
    }
    return false;
}

void GeX::process_definition()
{
    auto error1 = [&]{
        return GeXError(M_group_index, M_string_index,
                "Expected control sequence");
    };
    auto name_token = read_token();
    if (!name_token) error1();
    auto name = std::visit(overloaded{
        [&](CharacterToken&) -> std::u32string {
            throw error1();
            return U"";
        },
        [&](CommandToken& command_token) {
            return command_token.command;
        }
    }, name_token->value);

    // TODO: Parameters
    auto delimiters = TokenList();
    bool last_space = false;
    while (true) {
        auto token = read_token();
        if (!token) {
            throw GeXError(M_group_index, M_string_index,
                "End of input reached while processing a definition");
        }
        if (auto tok = std::get_if<CharacterToken>(&token->value)) {
            if (tok->catcode == CategoryCode::StartGroup) break;
            else if (tok->catcode == CategoryCode::EndGroup) {
                throw GeXError(M_group_index, M_string_index,
                        "Unexpected end group token");
            }
            else if (tok->catcode == CategoryCode::Spacer) {
                if (last_space) continue;
                last_space = true;
                tok->codepoint = U' ';
            }
            else last_space = false;
        }
        else last_space = false;
        delimiters.push_back(*token);
    }

    auto group_level = 1;
    auto command_list = TokenList();
    while (group_level != 0) {
        auto token = read_token();
        if (!token) {
            throw GeXError(M_group_index, M_string_index,
                "End of input reached while processing a definition");
        }
        if (auto tok = std::get_if<CharacterToken>(&token->value)) {
            if (tok->catcode == CategoryCode::StartGroup) ++group_level;
            else if (tok->catcode == CategoryCode::EndGroup) --group_level;
        }
        // Don't add the final end group
        if (group_level != 0) {
            command_list.push_back(*token);
        }
    }
    M_macros[name] = Macro(std::move(delimiters), std::move(command_list));
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
            return Token(read_escape(), M_group_index);
        }
        else return Token(
            CharacterToken(codepoint, get_category_code(codepoint)),
            M_group_index
        );
    });
}

GeX::CommandToken GeX::read_escape()
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
    return CommandToken(name, std::string(name_utf8));
}

GeX::CategoryCode GeX::get_category_code(std::uint32_t codepoint)
{
    for (int i = 0; i < 16; ++i) {
        if (i == 12) continue;
        if (M_catcodes[i].contains(codepoint)) return CategoryCode(i);
    }
    return CategoryCode::Other;
}
