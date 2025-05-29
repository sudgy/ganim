#include "preprocess.hpp"

#include <unordered_map>

#include "ganim/unicode.hpp"
#include "ganim/unicode_categories.hpp"
#include "ganim/object/text/text_helpers.hpp"

#include "gex_error.hpp"
#include "category_code.hpp"
#include "token.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
    class Preprocessor {
        public:
            explicit Preprocessor(const std::vector<std::string>& input);
            TokenList get_output();

        private:

            struct Macro {
                TokenList delimiters;
                TokenList replacement_text;
            };

            std::optional<std::uint32_t> read_character();
            void process_character_token(
                CharacterToken tok,
                int group,
                int string_index
            );
            void process_command_token(
                CommandToken tok,
                int group,
                int string_index
            );
            bool process_built_in(const std::u32string& command, int group);
            void process_definition();
            std::u32string process_definition_name();
            std::pair<TokenList, int> process_definition_delimiters();
            TokenList process_definition_replacement(int parameter_number);
            std::optional<Token> read_token();
            CommandToken read_escape();
            ParameterToken read_parameter_token();
            CategoryCode get_category_code(std::uint32_t codepoint);
            GeXError make_error(std::string_view what) const;

            std::vector<std::unordered_set<std::uint32_t>> M_catcodes;
            std::vector<std::string> M_input;
            TokenList M_output;
            std::unordered_map<std::u32string, Macro> M_macros;
            TokenList M_next_tokens;
            int M_group_index = 0;
            int M_string_index = 0;
            int M_last_group_index = -1;
            int M_last_string_index = -1;
            bool M_expanding = false;
    };
}

Preprocessor::Preprocessor(const std::vector<std::string>& input) : M_input(input)
{
    if (M_input.size() == 0) throw std::runtime_error("No input passed to Preprocessor");
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

TokenList Preprocessor::get_output()
{
    while (auto token = read_token()) {
        std::visit(overloaded{
            [&](CharacterToken& tok)
                {process_character_token(
                        tok, token->group, token->string_index);},
            [&](CommandToken& tok)
                {process_command_token(
                        tok, token->group, token->string_index);},
            [&](ParameterToken&)
                {throw make_error("Unexpected parameter token");}
        }, token->value);
    }
    return M_output;
}

void Preprocessor::process_character_token(
    CharacterToken tok,
    int group,
    int string_index
)
{
    M_output.emplace_back(tok, group, string_index);
}

void Preprocessor::process_command_token(
    CommandToken tok,
    int group,
    int string_index
)
{
    auto it = M_macros.find(tok.command);
    if (it == M_macros.end()) {
        throw make_error(
            std::format("Undefined control sequence \"{}\"", tok.command_utf8));
    }
    auto& macro = it->second;
    int delim_index = 0;
    bool last_space = false;
    auto parameters = std::vector<TokenList>();
    auto get_token = [&] -> Token {
        while (true) {
            auto token = read_token();
            if (!token) {
                throw make_error(
                    std::format("Input ended while processing macro \"{}\"",
                        tok.command_utf8));
            }
            if (auto tok = std::get_if<CharacterToken>(&token->value)) {
                if (tok->catcode == CategoryCode::Spacer) {
                    if (last_space) continue;
                    last_space = true;
                    tok->codepoint = U' ';
                }
                else last_space = false;
            }
            else last_space = false;
            return *token;
        }
    };
    // Read initial delimiters
    while (delim_index < ssize(macro.delimiters)) {
        if (std::holds_alternative<ParameterToken>(
                    macro.delimiters[delim_index].value)) {
            break;
        }
        auto token = get_token();
        if (token.value != macro.delimiters[delim_index].value) {
            throw make_error(
                std::format("Use of \\{} does not match its definition",
                    tok.command_utf8));
        }
        ++delim_index;
    }
    // Read parameters
    while (delim_index < ssize(macro.delimiters)) {
        auto& parameter_token = std::get<ParameterToken>(
                macro.delimiters[delim_index].value);
        auto& parameter_tokens = parameters.emplace_back();
        auto group_level = 0;
        auto common_process = [&] -> Token {
            auto token = get_token();
            if (auto tok = std::get_if<CharacterToken>(&token.value)) {
                if (tok->codepoint == U'{') ++group_level;
                if (tok->codepoint == U'}') --group_level;
            }
            if (group_level < 0) {
                throw make_error("Unexpected end of group");
            }
            parameter_tokens.push_back(token);
            return token;
        };
        if (parameter_token.delimited) {
            auto delims = TokenList();
            ++delim_index;
            while (delim_index < ssize(macro.delimiters)) {
                auto& token = macro.delimiters[delim_index];
                if (std::holds_alternative<ParameterToken>(token.value)) {
                    break;
                }
                else {
                    delims.push_back(token);
                    ++delim_index;
                }
            }
            while (true) {
                common_process();
                if (group_level == 0) {
                    auto diff = ssize(parameter_tokens) - ssize(delims);
                    if (diff >= 0) {
                        bool all_equal = true;
                        for (int i = 0; i < ssize(delims); ++i) {
                            if (parameter_tokens[i + diff] != delims[i]) {
                                all_equal = false;
                                break;
                            }
                        }
                        if (all_equal) break;
                    }
                }
            }
            parameter_tokens.resize(parameter_tokens.size() - delims.size());
        }
        // Undelimited token
        else {
            do {
                auto token = common_process();
                if (auto tok = std::get_if<CharacterToken>(&token.value)) {
                    if (tok->catcode == CategoryCode::Spacer) {
                        parameter_tokens.clear();
                    }
                }
            } while (group_level != 0 and !parameter_tokens.empty());
            ++delim_index;
        }
        // Kill outer group
        if (!parameter_tokens.empty()) {
            auto tok1 = std::get_if<CharacterToken>(
                    &parameter_tokens.front().value);
            auto tok2 = std::get_if<CharacterToken>(
                    &parameter_tokens.back().value);
            if (tok1 and tok2 and tok1->catcode == CategoryCode::StartGroup
                              and tok2->catcode == CategoryCode::EndGroup) {
                parameter_tokens.pop_front();
                parameter_tokens.pop_back();
            }
        }
    }
    if (macro.replacement_text.empty()) {
        if (process_built_in(tok.command, group)) return;
    }
    auto actual_expansion = TokenList();
    auto add_replacement_text = [&](int start, int end) {
        if (start != end) {
            auto i = ssize(actual_expansion);
            actual_expansion.insert(
                actual_expansion.end(),
                macro.replacement_text.begin() + start,
                macro.replacement_text.begin() + end
            );
            for (; i < ssize(actual_expansion); ++i) {
                actual_expansion[i].group = group;
                actual_expansion[i].string_index = string_index;
            }
        }
    };
    auto start = 0;
    for (int i = 0; i < ssize(macro.replacement_text); ++i) {
        if (auto param_tok = std::get_if<ParameterToken>(
                    &macro.replacement_text[i].value)) {
            add_replacement_text(start, i);
            actual_expansion.append_range(parameters[param_tok->number - 1]);
            start = i + 1;
        }
    }
    add_replacement_text(start, ssize(macro.replacement_text));
    M_next_tokens.prepend_range(actual_expansion);
}

bool Preprocessor::process_built_in(const std::u32string& command, int group)
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

void Preprocessor::process_definition()
{
    auto name = process_definition_name();
    auto [delimiters, parameter_number] = process_definition_delimiters();
    auto command_list = process_definition_replacement(parameter_number);
    M_macros[name] = Macro(std::move(delimiters), std::move(command_list));
}

std::u32string Preprocessor::process_definition_name()
{
    auto error = [&]{
        return make_error("Expected control sequence");
    };
    auto name_token = read_token();
    if (!name_token) throw error();
    return std::visit(overloaded{
        [&](CommandToken& command_token) {
            return command_token.command;
        },
        [&](auto&) -> std::u32string {
            throw error();
        }
    }, name_token->value);
}

std::pair<TokenList, int> Preprocessor::process_definition_delimiters()
{
    auto delimiters = TokenList();
    bool last_space = false;
    int last_parameter = 0;
    while (true) {
        auto token = read_token();
        if (!token) {
            throw make_error(
                "End of input reached while processing a definition");
        }
        if (auto tok = std::get_if<CharacterToken>(&token->value)) {
            if (tok->catcode == CategoryCode::StartGroup) break;
            else if (tok->catcode == CategoryCode::EndGroup) {
                throw make_error("Unexpected end group token");
            }
            else if (tok->catcode == CategoryCode::Spacer) {
                if (last_space) continue;
                last_space = true;
                tok->codepoint = U' ';
            }
            else last_space = false;
        }
        else {
            last_space = false;
            if (auto tok = std::get_if<ParameterToken>(&token->value)) {
                if (tok->number != last_parameter + 1) {
                    throw make_error(
                            "Parameters must be numbered consecutively");
                }
                last_parameter = tok->number;
            }
        }
        delimiters.push_back(*token);
    }
    for (int i = 0; i < ssize(delimiters); ++i) {
        if (auto tok = std::get_if<ParameterToken>(&delimiters[i].value)) {
            if (i == ssize(delimiters) - 1) {
                tok->delimited = false;
            }
            else if (std::holds_alternative<ParameterToken>(
                        delimiters[i+1].value)) {
                tok->delimited = false;
            }
        }
    }
    return {delimiters, last_parameter};
}

TokenList Preprocessor::process_definition_replacement(int parameter_number)
{
    auto group_level = 1;
    auto command_list = TokenList();
    while (group_level != 0) {
        auto token = read_token();
        if (!token) {
            throw make_error(
                "End of input reached while processing a definition");
        }
        if (auto tok = std::get_if<CharacterToken>(&token->value)) {
            if (tok->catcode == CategoryCode::StartGroup) ++group_level;
            else if (tok->catcode == CategoryCode::EndGroup) --group_level;
        }
        if (auto tok = std::get_if<ParameterToken>(&token->value)) {
            if (tok->number > parameter_number) {
                throw make_error("Illegal parameter number");
            }
        }
        // Don't add the final end group
        if (group_level != 0) {
            command_list.push_back(*token);
        }
    }
    return command_list;
}

std::optional<std::uint32_t> Preprocessor::read_character()
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

std::optional<Token> Preprocessor::read_token()
{
    if (!M_next_tokens.empty()) {
        M_expanding = true;
        auto result = M_next_tokens.front();
        M_next_tokens.pop_front();
        return result;
    }
    M_expanding = false;
    M_last_group_index = M_group_index;
    M_last_string_index = M_string_index;
    return read_character().transform([&](auto codepoint) {
        if (M_last_group_index != M_group_index) {
            ++M_last_group_index;
            M_last_string_index = 0;
        }
        auto category_code = get_category_code(codepoint);
        if (category_code == CategoryCode::Escape) {
            return Token(read_escape(), M_last_group_index,M_last_string_index);
        }
        else if (category_code == CategoryCode::MacroParameter) {
            return Token(
                read_parameter_token(),
                M_last_group_index,
                M_last_string_index
            );
        }
        else return Token(
            CharacterToken(codepoint, get_category_code(codepoint)),
            M_last_group_index,
            M_last_string_index
        );
    });
}

CommandToken Preprocessor::read_escape()
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

ParameterToken Preprocessor::read_parameter_token()
{
    auto number = read_character();
    auto error = [&]{
        return make_error(
            "Expected integer from 1 to 9 after macro parameter token");
    };
    if (!number) throw error();
    if (*number >= U'1' and *number <= U'9') {
        return ParameterToken(*number - U'0', true);
    }
    else throw error();
}

CategoryCode Preprocessor::get_category_code(std::uint32_t codepoint)
{
    for (int i = 0; i < 16; ++i) {
        if (i == 12) continue;
        if (M_catcodes[i].contains(codepoint)) return CategoryCode(i);
    }
    return CategoryCode::Other;
}

GeXError Preprocessor::make_error(std::string_view what) const
{
    auto group_index = M_last_group_index;
    auto string_index = M_last_string_index;
    if (group_index == ssize(M_input)) {
        --group_index;
        string_index = ssize(M_input.back());
    }
    if (M_expanding) {
        // Yes I'm passing a temporary string as a string_view, but the string
        // is immediately copied in PreprocessorError
        auto new_what = std::format("Error during macro expansion: {}", what);
        return GeXError(group_index, string_index, new_what);
    }
    else return GeXError(group_index, string_index, what);
}

TokenList ganim::gex::preprocess(const std::vector<std::string>& input)
{
    auto preprocessor = Preprocessor(input);
    return preprocessor.get_output();
}
