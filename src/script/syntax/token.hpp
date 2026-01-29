#ifndef GANIM_SCRIPT_SYNTAX_TOKEN_HPP
#define GANIM_SCRIPT_SYNTAX_TOKEN_HPP

#include <cstdint>
#include <string>

namespace ganim::syntax {
    struct IntToken {
        std::int64_t value = 0;
        int line_number = -1;
        int column_number = -1;
    };
    struct DoubleToken {
        double value = 0.0;
        int line_number = -1;
        int column_number = -1;
    };
    struct BooleanToken {
        bool value = false;
        int line_number = -1;
        int column_number = -1;
    };
    struct StringToken {
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct IdentifierToken {
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct AssignmentToken { // +=, -=, *=, /=
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct BoolOpToken { // ^, nand, nor
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct EqToken { // ==, !=
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct RelToken { // <, <=, >, >=
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct SumToken { // +, -
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
    struct ProdToken { // *, /, %
        std::string_view value;
        int line_number = -1;
        int column_number = -1;
    };
}

#endif
