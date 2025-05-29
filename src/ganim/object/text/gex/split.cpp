#include "split.hpp"

#include "gex_error.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<Section> gex::split(const TokenList& tokens)
{
    auto result = std::vector<Section>();
    int start = 0;
    bool math_mode = false;
    bool display = false;
    int dollars_seen = 0;
    auto add_new_section = [&](int end){
        if (start == end) return;
        if (start == 0 and end == ssize(tokens)) {
            result.push_back({tokens, Section::Text});
        }
        else {
            auto type = math_mode ?
                            (display ?
                                 Section::DisplayMath :
                                 Section::InlineMath) :
                            Section::Text;
            result.push_back({
                {tokens.begin() + start, tokens.begin() + end},
                type
            });
        }
    };
    for (int i = 0; i < ssize(tokens); ++i) {
        auto& token = tokens[i];
        if (auto tok = std::get_if<CharacterToken>(&token.value)) {
            if (tok->catcode == CategoryCode::MathShift) {
                if (dollars_seen == 0) {
                    add_new_section(i);
                }
                ++dollars_seen;
                if (dollars_seen == 3) {
                    throw GeXError(
                        token.group,
                        token.string_index,
                        "Three math shift characters in a row are not "
                            "permitted."
                    );
                }
            }
            else if (dollars_seen != 0) {
                if (dollars_seen == 1) display = false;
                else display = true;
                dollars_seen = 0;
                start = i;
                math_mode = !math_mode;
            }
        }
    }
    if (math_mode) {
        if (dollars_seen == 0 or (dollars_seen == 1 and display)) {
            throw GeXError(tokens.back().group, tokens.back().string_index,
                    "Expected math shift character");
        }
    }
    if (dollars_seen == 0) {
        add_new_section(ssize(tokens));
    }
    return result;
}
