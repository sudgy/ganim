#include "make_math_list.hpp"

#include "ganim/unicode_categories.hpp"
#include "gex_error.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

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

void process_character_token(
    const TokenList& tokens,
    int& i,
    const CharacterToken& tok,
    MathList& result
)
{
    if (tok.codepoint == U' ') return;
    if (tok.catcode == CategoryCode::StartGroup) {
        int group_level = 1;
        int j = i + 1;
        for (; j < ssize(tokens); ++j) {
            if (auto tok = get_if<CharacterToken>(&tokens[j].value)) {
                if (tok->catcode == CategoryCode::StartGroup) {
                    ++group_level;
                }
                else if (tok->catcode == CategoryCode::EndGroup) {
                    --group_level;
                    if (group_level == 0) break;
                }
            }
        }
        auto new_list = TokenList();
        for (int k = i + 1; k < j; ++k) {
            new_list.push_back(tokens[k]);
        }
        auto noad = Noad(
            Atom(
                Box(),
                AtomType::Ord,
                AtomField(
                    AtomFieldList(make_math_list(new_list)),
                    Box()
                )
            ),
            tokens[i].group,
            tokens[i].string_index
        );
        result.push_back(std::move(noad));
        i = j;
    }
    else {
        auto atom_type = get_atom_type(tok.codepoint);
        auto atom_nucleus
            = AtomField(AtomFieldSymbol(tok.codepoint), Box());
        auto atom = Atom(Box(), atom_type, atom_nucleus);
        auto noad = Noad(atom, tokens[i].group, tokens[i].string_index);
        result.push_back(std::move(noad));
    }
}

}


MathList gex::make_math_list(const TokenList& tokens)
{
    auto result = MathList();
    for (int i = 0; i < ssize(tokens); ++i) {
        auto& token = tokens[i];
        std::visit(overloaded{
            [&](const CharacterToken& tok)
            {
                process_character_token(tokens, i, tok, result);
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
