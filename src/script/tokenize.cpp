#include "tokenize.hpp"

#include <cstdint>

#include "ganim.hpp"

#include "script/unicode_categories.hpp"

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
    bool in_string = 0;

    auto string_view = std::string_view(string);
    auto full_string_view = std::string_view(string);
    while (string_view.size() > 0) {
        auto codepoint = utf8_get_next_codepoint(string_view, &byte_size);
        if (in_string) {
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
                in_string = false;
            }
        }
        else {
            const bool in_token = token_start != byte_number;
            if (is_whitespace(codepoint)) {
                if (in_token) {
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
                }
                else {
                    token_start += byte_size;
                    token_line = line_number;
                    token_column = column_number + 1;
                }
            }
            else if (is_identifier_continue(codepoint)) {
                if (in_token) {
                    // Do nothing
                }
                else {
                    // Do nothing
                }
            }
            else if (codepoint == '"') {
                if (in_token) {
                    result.push_back({
                        full_string_view.substr(
                                token_start, byte_number - token_start),
                        token_line,
                        token_column,
                        token_start
                    });
                    token_start = byte_number;
                    token_column = column_number;
                }
                in_string = true;
            }
            // Other character, like an operator
            else {
                if (in_token) {
                    result.push_back({
                        full_string_view.substr(
                                token_start, byte_number - token_start),
                        token_line,
                        token_column,
                        token_start
                    });
                    result.push_back({
                        full_string_view.substr(byte_number, byte_size),
                        token_line,
                        column_number,
                        byte_number
                    });
                    token_start = byte_number + byte_size;
                    token_line = line_number;
                    token_column = column_number + 1;
                }
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
            }
        }
        ++column_number;
        if (codepoint == '\n') {
            ++line_number;
            column_number = 0;
            if (!in_string) {
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
