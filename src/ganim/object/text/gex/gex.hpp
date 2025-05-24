#ifndef GANIM_GEX_HPP
#define GANIM_GEX_HPP

#include <variant>
#include <unordered_set>
#include <unordered_map>
#include <format>
#include <deque>

#include "ganim/object/text/text_helpers.hpp"

namespace ganim {
    class GeXError : public std::exception {
        public:
            GeXError(int group, int index, std::string_view actual_what)
            {
                M_what = std::format(
                    "GeX compilation error in group {} index {}: {}",
                    group, index, actual_what
                );
            }
            virtual const char* what() const noexcept
            {
                return M_what.c_str();
            }
        private:
            std::string M_what;
    };
    class GeX {
        public:
            explicit GeX(const std::vector<std::string>& input);

            std::vector<PositionedGlyph> get_output();

        private:
            enum class CategoryCode {
                Escape = 0,
                StartGroup = 1,
                EndGroup = 2,
                MathShift = 3,
                Alignment = 4,
                EndLine = 5,
                MacroParameter = 6,
                Superscript = 7,
                Subscript = 8,
                Ignored = 9,
                Spacer = 10,
                Letter = 11,
                Other = 12,
                Active = 13,
                Comment = 14,
                Invalid = 15
            };
            struct CharacterToken {
                std::uint32_t codepoint = 0;
                CategoryCode catcode = CategoryCode::Other;
                bool operator==(const CharacterToken&) const=default;
            };
            struct CommandToken {
                std::u32string command;
                std::string command_utf8;
                bool operator==(const CommandToken&) const=default;
            };
            struct ParameterToken {
                int number = -1;
                bool delimited = true;
                bool operator==(const ParameterToken&) const=default;
            };

            struct Token {
                std::variant<CharacterToken, CommandToken, ParameterToken>
                    value;
                int group = -1;
                bool operator==(const Token&) const=default;
            };
            using TokenList = std::deque<Token>;

            struct Macro {
                TokenList delimiters;
                TokenList replacement_text;
            };

            std::optional<std::uint32_t> read_character();
            void process_character_token(CharacterToken tok, int group);
            void process_command_token(CommandToken tok, int group);
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
            std::vector<std::pair<std::u32string, int>> M_output_codepoints;
            std::unordered_map<std::u32string, Macro> M_macros;
            TokenList M_next_tokens;
            int M_group_index = 0;
            int M_string_index = 0;
            int M_last_group_index = -1;
            int M_last_string_index = -1;
            bool M_expanding = false;
    };
}

#endif
