#ifndef GANIM_SCRIPT_TOKENIZE_HPP
#define GANIM_SCRIPT_TOKENIZE_HPP

#include <vector>
#include <string>

namespace ganim {
    struct Token {
        std::string_view string;
        int line_number = -1;
        int column_number = -1;
        int byte_number = -1;
        enum Type {
            Decimal,
            Hex,
            Octal,
            Binary,
            Double,
            String,
            Identifier,
            Else
        } type;
    };
    std::vector<Token> tokenize(std::string_view string);
}

#endif
