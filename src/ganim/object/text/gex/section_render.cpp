#include "section_render.hpp"

using namespace ganim;
using namespace ganim::gex;

RenderedSection gex::section_render(const Section& section)
{
    if (section.type == Section::Text) {
        auto& font = get_font("fonts/NewCM10-Regular.otf");
        auto codepoints = std::vector<std::pair<std::u32string, int>>();
        for (auto& token : section.tokens) {
            if (codepoints.empty()) {
                codepoints.emplace_back(U"", token.group);
            }
            else {
                if (codepoints.back().second != token.group) {
                    codepoints.emplace_back(U"", token.group);
                }
            }
            codepoints.back().first.push_back(
                std::get<CharacterToken>(token.value).codepoint
            );
        }
        auto glyphs = shape_text_manual_groups(font, codepoints);
        return {glyphs, {}};
    }
    else {
        return {{}, {}};
    }
}
