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
        Integer10
    };
    auto state = None;
    enum CharacterType {
        Whitespace,
        IdentifierStart,
        IdentifierContinue, // Doesn't include numbers
        Number,
        Else
    };

    auto string_view = std::string_view(string);
    auto full_string_view = std::string_view(string);
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
                throw ScriptException(line_number, column_number,
                        "Invalid identifier");
            case Number:
                state = Integer10;
                break;
            case Else:
                if (codepoint == '"') state = String;
                else {
                    result.push_back({
                        full_string_view.substr(byte_number, byte_size),
                        token_line,
                        column_number,
                        byte_number
                    });
                    token_start += byte_size;
                    token_line = line_number;
                    token_column += 1;
                }
                break;
            }
            break;
        case String:
            if (codepoint == '"') {
                result.push_back({
                    full_string_view.substr(
                            token_start, byte_number - token_start + byte_size),
                    token_line,
                    token_column,
                    token_start
                });
                token_start = byte_number + byte_size;
                token_line = line_number;
                token_column = column_number + 1;
                state = None;
            }
            break;
        case Identifier:
            switch (type) {
            case Whitespace:
                result.push_back({
                    full_string_view.substr(
                            token_start, byte_number - token_start),
                    token_line,
                    token_column,
                    token_start
                });
                token_start = byte_number + byte_size;
                token_line = line_number;
                token_column = column_number + 1;
                state = None;
                break;
            case IdentifierStart:
            case IdentifierContinue:
            case Number:
                break;
            case Else:
                result.push_back({
                    full_string_view.substr(
                            token_start, byte_number - token_start),
                    token_line,
                    token_column,
                    token_start
                });
                token_start = byte_number;
                token_line = line_number;
                token_column = column_number;
                state = None;
                if (codepoint == '"') state = String;
                else {
                    result.push_back({
                        full_string_view.substr(byte_number, byte_size),
                        token_line,
                        column_number,
                        byte_number
                    });
                    token_start += byte_size;
                    token_line = line_number;
                    token_column += 1;
                }
                break;
            }
            break;
        case Integer10:
            switch (type) {
            case Whitespace:
                result.push_back({
                    full_string_view.substr(
                            token_start, byte_number - token_start),
                    token_line,
                    token_column,
                    token_start
                });
                token_start = byte_number + byte_size;
                token_line = line_number;
                token_column = column_number + 1;
                state = None;
                break;
            case IdentifierStart:
            case IdentifierContinue:
                throw ScriptException(token_line, token_column,
                        "Invalid numeric literal");
            case Number:
                break;
            case Else:
                result.push_back({
                    full_string_view.substr(
                            token_start, byte_number - token_start),
                    token_line,
                    token_column,
                    token_start
                });
                token_start = byte_number;
                token_line = line_number;
                token_column = column_number;
                state = None;
                if (codepoint == '"') state = String;
                else {
                    result.push_back({
                        full_string_view.substr(byte_number, byte_size),
                        token_line,
                        column_number,
                        byte_number
                    });
                    token_start += byte_size;
                    token_line = line_number;
                    token_column += 1;
                }
                break;
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
