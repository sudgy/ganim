#include "tokenize.hpp"

#include <cstdint>

#include "ganim.hpp"

#include "script/unicode_categories.hpp"
#include "script/script_exception.hpp"

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

using namespace ganim;

std::vector<Token> ganim::tokenize(std::string_view string)
{
    auto result = std::vector<Token>();

    int byte_size = 0;

    int line_number = 0;
    int column_number = 0;
    int byte_number = 0;

    int token_start = 0;
    int token_column = 0;
    int token_line = 0;

    enum State {
        None,
        String,
        Identifier,
        Integer10,
        MaybeFloat,
        Float
    };
    enum FloatState {
        BeforePoint,
        AfterPoint,
        AfterE,
        Ending
    };
    auto state = None;
    auto float_state = BeforePoint;
    enum CharacterType {
        Whitespace,
        IdentifierStart,
        IdentifierContinue, // Doesn't include numbers
        Number,
        Else
    };

    auto string_view = std::string_view(string);
    auto full_string_view = std::string_view(string);

    auto add_token = [&](
            bool include_current_character,
            bool is_identifier = false
        ) {
        auto size = byte_number - token_start;
        if (include_current_character) size += byte_size;
        result.push_back({
            full_string_view.substr(token_start, size),
            token_line,
            token_column,
            token_start,
            is_identifier
        });
        token_start = byte_number + byte_size;
        token_line = line_number;
        token_column = column_number + 1;
    };

    while (string_view.size() > 0) {
        auto codepoint = utf8_get_next_codepoint(string_view, &byte_size);
        const auto type = [](auto c) {
            if ('0' <= c and c <= '9') return Number;
            if (is_whitespace(c)) return Whitespace;
            if (is_identifier_start(c)) return IdentifierStart;
            if (is_identifier_continue(c)) return IdentifierContinue;
            return Else;
        }(codepoint);
        switch (state) {
        case None:
            switch (type) {
            case Whitespace:
                token_start += byte_size;
                token_line = line_number;
                token_column = column_number + 1;
                break;
            case IdentifierStart:
                state = Identifier;
                break;
            case IdentifierContinue:
                throw CompileError(line_number, column_number,
                        "Invalid identifier");
            case Number:
                state = Integer10;
                break;
            case Else:
                if (codepoint == '"') state = String;
                else if (codepoint == '.') state = MaybeFloat;
                else add_token(true);
                break;
            }
            break;
        case String:
            if (codepoint == '"') {
                add_token(true);
                state = None;
            }
            break;
        case Identifier:
            switch (type) {
            case Whitespace:
                add_token(false, true);
                state = None;
                break;
            case IdentifierStart:
            case IdentifierContinue:
            case Number:
                break;
            case Else:
                add_token(false, true);
                token_start = byte_number;
                token_column = column_number;
                state = None;
                if (codepoint == '"') state = String;
                else if (codepoint == '.') state = MaybeFloat;
                else add_token(true);
                break;
            }
            break;
        case Integer10:
            switch (type) {
            case Whitespace:
                add_token(false);
                state = None;
                break;
            case IdentifierStart:
            case IdentifierContinue:
                if (codepoint == 'e') {
                    state = Float;
                    float_state = AfterE;
                }
                else throw CompileError(token_line, token_column,
                        "Invalid numeric literal");
            case Number:
                break;
            case Else:
                if (codepoint == '.') {
                    state = Float;
                    float_state = AfterPoint;
                }
                else {
                    add_token(false);
                    token_start = byte_number;
                    token_column = column_number;
                    state = None;
                    if (codepoint == '"') state = String;
                    else add_token(true);
                }
                break;
            }
            break;
        case MaybeFloat:
            switch (type) {
            case Whitespace:
                add_token(false);
                state = None;
                break;
            case IdentifierStart:
                add_token(false);
                break;
            case IdentifierContinue:
                throw CompileError(line_number, column_number,
                        "Invalid identifier");
            case Number:
                state = Float;
                float_state = AfterPoint;
                break;
            case Else:
                add_token(false);
                token_start = byte_number;
                token_column = column_number;
                state = None;
                if (codepoint == '"') state = String;
                else add_token(true);
                break;
            }
            break;
        case Float:
            switch (type) {
            case Whitespace:
                add_token(false);
                state = None;
                break;
            case IdentifierStart:
                if (float_state == AfterPoint and codepoint == 'e') {
                    float_state = AfterE;
                }
                else {
                    throw CompileError(token_line, token_column,
                            "Invalid floating-point literal");
                }
                break;
            case IdentifierContinue:
                throw CompileError(token_line, token_column,
                        "Invalid floating-point literal");
            case Number:
                if (float_state == AfterE) float_state = Ending;
                break;
            case Else:
                if (codepoint == '.') {
                    throw CompileError(token_line, token_column,
                            "Invalid floating-point literal");
                }
                if (codepoint == '+' or codepoint == '-') {
                    if (float_state == AfterE) {
                        float_state = Ending;
                    }
                    else {
                        throw CompileError(token_line, token_column,
                                "Invalid floating-point literal");
                    }
                }
                else {
                    add_token(false);
                    token_start = byte_number;
                    token_column = column_number;
                    state = None;
                    if (codepoint == '"') state = String;
                    else add_token(true);
                }
            }
            break;
        }
        ++column_number;
        if (codepoint == '\n') {
            ++line_number;
            column_number = 0;
            if (state != String) {
                token_line = line_number;
                token_column = column_number;
            }
        }
        string_view.remove_prefix(byte_size);
        byte_number += byte_size;
    }
    if (token_start != byte_number) {
        result.push_back({
            full_string_view.substr(
                    token_start, byte_number - token_start),
            token_line,
            token_column,
            token_start
        });
    }
    return result;
}
