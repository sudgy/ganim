#include "section_render.hpp"

#include "make_math_list.hpp"
#include "render_math_list.hpp"

using namespace ganim;
using namespace ganim::gex;

Box gex::section_render(const Section& section)
{
    if (section.type == Section::Text) {
        auto& font = get_font("fonts/NewCM10-Regular.otf");
        auto codepoints = std::vector<std::pair<std::u32string, int>>();
        for (auto& token : section.tokens) {
            if (auto tok = get_if<CharacterToken>(&token.value)) {
                if (tok->catcode == CategoryCode::StartGroup or
                    tok->catcode == CategoryCode::EndGroup) continue;
                if (codepoints.empty()) {
                    codepoints.emplace_back(U"", token.group);
                }
                else {
                    if (codepoints.back().second != token.group) {
                        codepoints.emplace_back(U"", token.group);
                    }
                }
                codepoints.back().first.push_back(tok->codepoint);
            }
        }
        auto glyphs = shape_text_manual_groups(font, codepoints);
        return box_from_glyphs(glyphs);
    }
    else {
        auto style = section.type == Section::InlineMath
            ? Style::Text : Style::Display;
        return render_math_list(make_math_list(section.tokens), style);
    }
}
