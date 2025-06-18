#include "gex.hpp"

#include "ganim/fmap.hpp"

#include "preprocess.hpp"
#include "split.hpp"
#include "section_render.hpp"
#include "section_combine.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<PositionedGlyph>
ganim::gex_render(const std::vector<std::string_view>& input)
{
    auto tokens = preprocess(input);
    auto sections = split(tokens);
    auto rendered_sections = fmap(sections, section_render);
    return section_combine(rendered_sections);
}
