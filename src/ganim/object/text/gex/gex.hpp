#ifndef GANIM_GEX_HPP
#define GANIM_GEX_HPP

#include "ganim/object/text/text_helpers.hpp"

namespace ganim {
    class GeX {
        public:
            explicit GeX(const std::vector<std::string>& input);

            void tokenize();
            std::vector<PositionedGlyph> get_output();

        private:
            struct CharacterToken {
                std::uint32_t codepoint = 0;
            };

            std::vector<std::string> M_input;
            std::vector<CharacterToken> M_tokens;
    };
}

#endif
