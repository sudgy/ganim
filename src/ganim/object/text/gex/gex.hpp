#ifndef GANIM_GEX_HPP
#define GANIM_GEX_HPP

#include <variant>

#include "ganim/object/text/text_helpers.hpp"

namespace ganim {
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
            };

            struct Token {
                std::variant<CharacterToken> value;
                int group = -1;
            };

            std::optional<Token> read_token();

            std::vector<std::string> M_input;
            int M_group_index = 0;
            int M_string_index = 0;
    };
}

#endif
