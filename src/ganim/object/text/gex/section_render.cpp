#include "section_render.hpp"

#include "make_math_list.hpp"
#include "render_math_list.hpp"

using namespace ganim;
using namespace ganim::gex;

Box gex::section_render(Section section)
{
    if (section.type == Section::Text) {
        auto& font = get_font("fonts/NewCM10-Regular.otf");
        double space_amount = 0.0;
        while (!section.tokens.empty()) {
            if (auto tok=get_if<CharacterToken>(&section.tokens.back().value)) {
                if (tok->catcode == CategoryCode::Spacer) {
                    if (tok->codepoint == U' ') {
                        space_amount += get_font_em(font) / 3.0;
                        section.tokens.pop_back();
                        continue;
                    }
                }
            }
            break;
        }
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
        auto result = box_from_glyphs(glyphs);
        result.width += space_amount;
        return result;
    }
    else {
        auto style = section.type == Section::InlineMath
            ? Style::Text : Style::Display;
        return render_math_list(make_math_list(section.tokens), style);
    }
}
