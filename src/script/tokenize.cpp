#include "tokenize.hpp"

#include <cstdint>

#include "ganim.hpp"

#include "script/script_exception.hpp"

using namespace ganim;

namespace {
    bool is_identifier_start(std::uint32_t codepoint)
    {
        if (codepoint < 0x80) {
            return ('a' <= codepoint and codepoint <= 'z') or
                   ('A' <= codepoint and codepoint <= 'Z');
        }
        else return C_xid_start.contains(codepoint);
    }
    bool is_identifier_continue(std::uint32_t codepoint)
    {
        if (codepoint < 0x80) {
            return ('a' <= codepoint and codepoint <= 'z') or
                   ('A' <= codepoint and codepoint <= 'Z') or
                   ('0' <= codepoint and codepoint <= '9');
        }
        else return C_xid_continue.contains(codepoint);
    }
    bool is_whitespace(std::uint32_t codepoint)
    {
        if (codepoint < 0x80) {
            return (codepoint >= 0x9 and codepoint <= 0xD) or codepoint == ' ';
        }
        else return (codepoint == 0x0085 or codepoint == 0x200E
                  or codepoint == 0x200F or codepoint == 0x2028
                  or codepoint == 0x2029);
    }
}

std::vector<Token> ganim::tokenize(std::string_view string)
{
    auto result = std::vector<Token>();

    int byte_size = 0; // Size of the current character being looked at

    int start_byte = 0;
    int start_line = 0;
    int start_column = 0;

    int lookahead_byte = 0;
    int lookahead_line = 0;
    int lookahead_column = 0;

    enum State {
        Start,
        Number, // Started with 0, notably NOT decimal
        Hex1, // Expecting first digit
        Octal, // Continuing, can end
        Binary1, // Expecting first digit
        Hex2, // Continuing, can end
        Binary2, // Continuing, can end
        Decimal, // Continuing, can end
        Float1, // After a decimal and the .
        Float2, // Right after the e
        Float3, // After e+ or e-
        Float4, // After the e and at least one digit
        Float5, // Starting from ., could just be the . operator
        Identifier, // After the start of an identifier
        String1, // Inside a string literal
        String2, // While escaping anything in a string literal
        Operator, // Seen first character
        SingleLineComment, // After the //, waiting for line to end
        MultiLineComment, // After the /*, looking for */
        MultiLineCommentEnd // After then /*, and seen a *
    };
    auto state = Start;
    enum CharacterType {
        Whitespace,
        Digit,
        IdentifierStart,
        IdentifierContinue, // Doesn't include numbers
        Else
    };

    auto string_view = std::string_view(string);
    auto full_string_view = std::string_view(string);

    bool do_next = true;
    auto add_token = [&](
        bool include_current_character,
        Token::Type type
    )
    {
        auto size = lookahead_byte - start_byte;
        if (include_current_character) size += byte_size;
        result.push_back({
            full_string_view.substr(start_byte, size),
            start_line,
            start_column,
            start_byte,
            type
        });
        start_byte = lookahead_byte;
        start_line = lookahead_line;
        start_column = lookahead_column;
        if (include_current_character) {
            start_byte += byte_size;
            ++start_column;
        }
        state = Start;
        do_next = include_current_character;
    };

    while (true) {
        do_next = true;
        auto codepoint = string_view.empty() ?
            0 :
            utf8_get_next_codepoint(string_view, &byte_size);
        const auto type = [](auto c) {
            if ('0' <= c and c <= '9') return Digit;
            if (is_whitespace(c)) return Whitespace;
            if (is_identifier_start(c)) return IdentifierStart;
            if (is_identifier_continue(c)) return IdentifierContinue;
            return Else;
        }(codepoint);
        const auto identifier_character =
            type == IdentifierStart or type == IdentifierContinue;
        const auto valid_hex =
                ('0' <= codepoint and codepoint <= '9') or
                ('a' <= codepoint and codepoint <= 'f') or
                ('A' <= codepoint and codepoint <= 'F');
        const auto valid_oct = '0' <= codepoint and codepoint <= '7';
        switch (state) {
        case Start:
            switch (type) {
            case Whitespace:
                start_byte += byte_size;
                ++start_column;
                break;
            case Digit:
                if (codepoint == '0') {
                    state = Number;
                }
                else {
                    state = Decimal;
                }
                break;
            case IdentifierStart:
                state = Identifier;
                break;
            case IdentifierContinue:
                throw CompileError(start_line, start_column,
                        "Invalid identifier");
            case Else:
                if (codepoint == '"') {
                    state = String1;
                }
                else if (codepoint == '.') {
                    state = Float5;
                }
                else if (codepoint != 0) {
                    state = Operator;
                }
                break;
            }
            break;
        case Number:
            if (codepoint == 'x' or codepoint == 'X') {
                state = Hex1;
            }
            else if (codepoint == 'b' or codepoint == 'B') {
                state = Binary1;
            }
            else if (valid_oct) {
                state = Octal;
            }
            else if (codepoint == '.') {
                state = Float1;
            }
            else {
                throw CompileError(start_line, start_column,
                        "Invalid numeric literal");
            }
            break;
        case Hex1:
            if (valid_hex) {
                state = Hex2;
            }
            else {
                throw CompileError(start_line, start_column,
                        "Invalid hexadecimal literal");
            }
            break;
        case Octal:
            if (valid_oct) {
                // Keep going
            }
            else if (codepoint == '.') {
                state = Float1;
            }
            else if (identifier_character) {
                throw CompileError(start_line, start_column,
                        "Invalid octal literal");
            }
            else {
                add_token(false, Token::Octal);
            }
            break;
        case Binary1:
            if (codepoint == '0' or codepoint == '1') {
                state = Binary2;
            }
            else {
                throw CompileError(start_line, start_column,
                        "Invalid binary literal");
            }
            break;
        case Hex2:
            if (valid_hex) {
                // Keep going
            }
            else if (identifier_character) {
                throw CompileError(start_line, start_column,
                        "Invalid hexadecimal literal");
            }
            else {
                add_token(false, Token::Hex);
            }
            break;
        case Binary2:
            if (codepoint == '0' or codepoint == '1') {
                // Keep going
            }
            else if (identifier_character) {
                throw CompileError(start_line, start_column,
                        "Invalid binary literal");
            }
            else {
                add_token(false, Token::Binary);
            }
            break;
        case Decimal:
            if (type == Digit) {
                // Keep going
            }
            else if (codepoint == '.') {
                state = Float1;
            }
            else if (codepoint == 'e') {
                state = Float2;
            }
            else if (identifier_character) {
                throw CompileError(start_line, start_column,
                        "Invalid decimal literal");
            }
            else {
                add_token(false, Token::Decimal);
            }
            break;
        case Float1:
            if (type == Digit) {
                // Keep going
            }
            else if (codepoint == 'e') {
                state = Float2;
            }
            else if (codepoint == '.') {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            else if (identifier_character) {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            else {
                add_token(false, Token::Double);
            }
            break;
        case Float2:
            if (type == Digit) {
                state = Float4;
            }
            else if (codepoint == '+' or codepoint == '-') {
                state = Float3;
            }
            else if (codepoint == '.') {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            else {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            break;
        case Float3:
            if (type == Digit) {
                state = Float4;
            }
            else if (codepoint == '.') {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            else {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            break;
        case Float4:
            if (type == Digit) {
                // Keep going
            }
            else if (codepoint == '.') {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            else if (identifier_character) {
                throw CompileError(start_line, start_column,
                        "Invalid floating-point literal");
            }
            else {
                add_token(false, Token::Double);
            }
            break;
        case Float5:
            if (type == Digit) {
                state = Float1;
            }
            else {
                add_token(false, Token::Else);
            }
            break;
        case Identifier:
            if (type == IdentifierStart or type == IdentifierContinue
                or type == Digit)
            {
                // Keep going
            }
            else {
                add_token(false, Token::Identifier);
            }
            break;
        case String1:
            if (codepoint == '"') {
                add_token(true, Token::String);
            }
            else if (codepoint == '\\') {
                state = String2;
            }
            break;
        case String2:
            state = String1;
            break;
        case Operator:
        {
            bool compound = false;
            auto last_two = full_string_view.substr(
                start_byte,
                lookahead_byte - start_byte + byte_size
            );
            if (last_two == "//") {
                state = SingleLineComment;
                break;
            }
            else if (last_two == "/*") {
                state = MultiLineComment;
                break;
            }
            else if (last_two == "==") compound = true;
            else if (last_two == "!=") compound = true;
            else if (last_two == "<=") compound = true;
            else if (last_two == ">=") compound = true;
            else if (last_two == "&&") compound = true;
            else if (last_two == "||") compound = true;
            else if (last_two == "+=") compound = true;
            else if (last_two == "-=") compound = true;
            else if (last_two == "*=") compound = true;
            else if (last_two == "/=") compound = true;
            if (compound) {
                add_token(true, Token::Else);
            }
            else {
                add_token(false, Token::Else);
            }
            break;
        }
        case SingleLineComment:
            // This case is handled after the switch when it sees a newline
            break;
        case MultiLineComment:
            if (codepoint == '*') {
                state = MultiLineCommentEnd;
            }
            break;
        case MultiLineCommentEnd:
            if (codepoint == '/') {
                state = Start;
                start_byte = lookahead_byte + byte_size;
                start_line = lookahead_line;
                start_column = lookahead_column + 1;
            }
            else {
                state = MultiLineComment;
            }
            break;
        }
        if (do_next) {
            ++lookahead_column;
            if (codepoint == '\n') {
                ++lookahead_line;
                lookahead_column = 0;
                if (state == SingleLineComment) {
                    state = Start;
                }
                if (state != String1 and state != String2) {
                    start_byte = lookahead_byte + byte_size;
                    start_line = lookahead_line;
                    start_column = lookahead_column;
                }
            }
            if (codepoint == 0) break;
            string_view.remove_prefix(byte_size);
            lookahead_byte += byte_size;
        }
    }
    if (state != Start) {
        throw CompileError(lookahead_line, lookahead_column,
            "Bad token at end of file");
    }
    return result;
}
