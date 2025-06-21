#include "make_math_list.hpp"

#include "ganim/unicode_categories.hpp"
#include "gex_error.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

// I'm sure this is supposed to be configurable so this might change in the
// future
AtomType get_atom_type(std::int32_t codepoint)
{
    using enum AtomType;
    if (C_tex_op.contains(codepoint)) return Op;
    if (C_tex_bin.contains(codepoint)) return Bin;
    if (C_tex_rel.contains(codepoint)) return Rel;
    if (C_tex_open.contains(codepoint)) return Open;
    if (C_tex_close.contains(codepoint)) return Close;
    if (C_tex_punct.contains(codepoint)) return Punct;
    return Ord;
}

MathList gex::make_math_list(const Section& section)
{
    auto result = MathList();
    for (auto& token : section.tokens) {
        std::visit(overloaded{
            [&](const CharacterToken& tok)
            {
                if (tok.codepoint == U' ') return;
                auto atom_type = get_atom_type(tok.codepoint);
                auto atom_nucleus
                    = AtomField(AtomFieldSymbol(tok.codepoint), Box());
                auto atom = Atom(Box(), atom_type, atom_nucleus);
                auto noad = Noad(atom, token.group, token.string_index);
                result.push_back(noad);
            },
            [&](const CommandToken& tok)
            {
                result.emplace_back(
                    CommandNoad(tok.command_utf8),
                    token.group,
                    token.string_index
                );
            },
            [&](const ParameterToken&)
            {
                throw GeXError(token.group, token.string_index,
                        "Unexpected parameter token");
            },
        }, token.value);
    }
    return result;
}
