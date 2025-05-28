#ifndef GANIM_GEX_TOKEN_HPP
#define GANIM_GEX_TOKEN_HPP

#include <string>
#include <variant>
#include <deque>
#include <cstdint>

#include "category_code.hpp"

namespace ganim::gex {
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
}

#endif
