#include "gex.hpp"

#include "ganim/fmap.hpp"

#include "preprocess.hpp"
#include "split.hpp"
#include "section_render.hpp"
#include "section_combine.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<PositionedGlyph>
ganim::gex_render(bool math, const std::vector<std::string_view>& input)
{
    auto tokens = preprocess(math, input);
    auto sections = split(tokens);
    auto rendered_sections = fmap(
        sections,
        [&](const auto& section) {
            return section_render(section);
        });
    return section_combine(rendered_sections).glyphs;
}
