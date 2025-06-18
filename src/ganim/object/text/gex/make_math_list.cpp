#include "make_math_list.hpp"

using namespace ganim;
using namespace ganim::gex;

// I'm sure this is supposed to be configurable so this might change in the
// future
AtomType get_atom_type(std::int32_t codepoint)
{
    using enum AtomType;
    // Very temporary
    if (codepoint == U'=') return Rel;
    if (codepoint == U'+') return Bin;
    return Ord;
}

MathList gex::make_math_list(const Section& section)
{
    auto result = MathList();
    for (auto& token : section.tokens) {
        auto& tok = get<CharacterToken>(token.value);
        auto atom_type = get_atom_type(tok.codepoint);
        auto atom_nucleus = AtomField(AtomFieldSymbol(tok.codepoint), Box());
        auto atom = Atom(Box(), atom_type, atom_nucleus);
        auto noad = Noad(atom, token.group, token.string_index);
        result.push_back(noad);
    }
    return result;
}
