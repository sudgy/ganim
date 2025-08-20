#ifndef GANIM_GEX_PREPROCESSOR
#define GANIM_GEX_PREPROCESSOR

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "gex_error.hpp"
#include "token.hpp"
#include "macro.hpp"

namespace ganim::gex {
    class Preprocessor {
        public:
            Preprocessor(bool math);
            void process(const std::vector<std::string_view>& input);
            TokenList get_output();

        private:
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
            void process_expandafter();
            std::optional<Token> read_token();
            CommandToken read_escape();
            ParameterToken read_parameter_token();
            CategoryCode get_category_code(std::uint32_t codepoint);
            GeXError make_error(std::string_view what) const;

            std::vector<std::unordered_set<std::uint32_t>> M_catcodes;
            std::vector<std::string_view> M_input;
            TokenList M_output;
            MacroStack M_macros;
            TokenList M_next_tokens;
            int M_group_index = 0;
            int M_string_index = 0;
            int M_last_group_index = -1;
            int M_last_string_index = -1;
            bool M_expanding = false;
            bool M_starting_math = false;
    };
}

#endif
