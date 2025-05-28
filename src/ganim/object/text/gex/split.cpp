#include "split.hpp"

using namespace ganim;
using namespace ganim::gex;

std::vector<Section> gex::split(const TokenList& tokens)
{
    return {{tokens, Section::Text}};
}
