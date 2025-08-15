#include "make_math_list.hpp"

#include "ganim/unicode_categories.hpp"
#include "gex_error.hpp"
#include "gex_dimension_parser.hpp"

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

class Processor {
    public:
        Processor(const TokenList& tokens) : tokens(tokens) {}
        void process();
        void process_character_token(const CharacterToken& token);
        void process_command_token(const CommandToken& token);
        void process_normal(std::uint32_t codepoint);
        void process_group();
        void process_subscript();
        void process_superscript();
        void process_generalized_fraction(int group);

        MathList get_result() {return std::move(result);}

    private:
        const Token& current_token() {return tokens[token_index];}
        void add_noad(Noad noad);
        double read_dimension();

        const TokenList& tokens;
        int token_index = 0;
        MathList result;
        bool subscript = false;
        bool superscript = false;
        int accent = 0;
};

}

void Processor::process()
{
    while (token_index < ssize(tokens)) {
        auto& token = current_token();
        std::visit(overloaded{
            [&](const CharacterToken& tok)
            {
                process_character_token(tok);
            },
            [&](const CommandToken& tok)
            {
                process_command_token(tok);
            },
            [&](const ParameterToken&)
            {
                throw GeXError(token.group, token.string_index,
                        "Unexpected parameter token");
            },
        }, token.value);
        ++token_index;
    }
}

void Processor::process_character_token(const CharacterToken& tok)
{
    if (tok.codepoint == U' ') return;
    switch (tok.catcode) {
        case CategoryCode::StartGroup:
            process_group();
            break;
        case CategoryCode::Spacer:
            break;
        case CategoryCode::Subscript:
            process_subscript();
            break;
        case CategoryCode::Superscript:
            process_superscript();
            break;
        default:
            process_normal(tok.codepoint);
            break;
    }
}

void Processor::process_command_token(const CommandToken& tok)
{
    auto& token = current_token();
    if (tok.command_utf8 == "mathaccent") {
        add_noad(Noad(Atom(Box(), AtomType::Acc, AtomAccent())));
        accent = 2;
    }
    else if (tok.command_utf8 == "abovewithdelims") {
        process_generalized_fraction(token.group);
    }
    else {
        add_noad(Noad(CommandNoad(
            tok.command_utf8,
            token.group,
            token.string_index
        )));
    }
}

void Processor::process_normal(std::uint32_t codepoint)
{
    auto atom_type = get_atom_type(codepoint);
    auto& token = current_token();
    auto symbol = AtomSymbol(
        codepoint,
        token.group,
        token.string_index
    );
    auto atom = Atom(Box(), atom_type, std::move(symbol));
    add_noad(Noad(std::move(atom)));
}

void Processor::process_group()
{
    int group_level = 1;
    int i = token_index + 1;
    for (; i < ssize(tokens); ++i) {
        if (auto tok = get_if<CharacterToken>(&tokens[i].value)) {
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
    for (int j = token_index + 1; j < i; ++j) {
        new_list.push_back(tokens[j]);
    }
    auto noad = Noad(
        Atom(
            Box(),
            AtomType::Ord,
            AtomList(make_math_list(new_list))
        )
    );
    add_noad(std::move(noad));
    token_index = i;
}

void Processor::process_subscript()
{
    auto& token = current_token();
    if (subscript or superscript) {
        throw GeXError(token.group, token.string_index, "Unexpected subscript");
    }
    if (token_index + 1 == ssize(tokens)) {
        throw GeXError(token.group, token.string_index, "Expected atom");
    }
    subscript = true;
}

void Processor::process_superscript()
{
    auto& token = current_token();
    if (subscript or superscript) {
        throw GeXError(token.group, token.string_index,
                "Unexpected superscript");
    }
    if (token_index + 1 == ssize(tokens)) {
        throw GeXError(token.group, token.string_index, "Expected atom");
    }
    superscript = true;
}

void Processor::process_generalized_fraction(int group)
{
    auto get_delim = [&] -> std::uint32_t {
        while (true) {
            ++token_index;
            if (token_index == ssize(tokens)) {
                throw GeXError(-1, -1, "Expected character token");
            }
            auto& token = current_token();
            if (auto tok = get_if<CharacterToken>(&token.value)) {
                if (tok->catcode == CategoryCode::Spacer) {
                    continue;
                }
                if (tok->catcode != CategoryCode::Other) {
                    throw GeXError(token.group, token.string_index,
                        "Expected character token");
                }
                if (tok->codepoint == U'.') return 0;
                else return tok->codepoint;
            }
        }
    };
    auto delim1 = get_delim();
    auto delim2 = get_delim();
    ++token_index;
    auto rule_thickness = read_dimension();
    auto denom_list = TokenList();
    for ( ; token_index < ssize(tokens); ++token_index) {
        denom_list.push_back(current_token());
    }
    auto noad = FractionNoad(
        std::move(result),
        make_math_list(denom_list),
        delim1,
        delim2,
        rule_thickness,
        group
    );
    result = MathList{{noad}};
}

void Processor::add_noad(Noad noad)
{
    auto& token = current_token();
    if (subscript or superscript) {
        if (!holds_alternative<Atom>(noad.value)) {
            throw GeXError(token.group, token.string_index, "Expected atom");
        }
        if (result.empty()) {
            result.push_back(Noad(Atom(Box(), AtomType::Ord, AtomEmpty())));
        }
        else if (!holds_alternative<Atom>(result.back().value)) {
            result.push_back(Noad(Atom(Box(), AtomType::Ord, AtomEmpty())));
        }
    }
    if (subscript) {
        subscript = false;
        auto old_noad = std::move(result.back());
        result.pop_back();
        auto& atom = get<Atom>(old_noad.value);
        if (holds_alternative<AtomSubscript>(atom.value)) {
            throw GeXError(token.group, token.string_index, "Double subscript");
        }
        else if (holds_alternative<AtomSubsuperscript>(atom.value)) {
            throw GeXError(token.group, token.string_index, "Double subscript");
        }
        else if (auto super = get_if<AtomSuperscript>(&atom.value)) {
            add_noad(Noad(Atom(Box(), super->nucleus->type, AtomSubsuperscript(
                std::move(super->nucleus),
                std::make_unique<Atom>(get<Atom>(noad.value)),
                std::move(super->superscript)
            ))));
        }
        else {
            add_noad(Noad(Atom(Box(), atom.type, AtomSubscript(
                std::make_unique<Atom>(std::move(atom)),
                std::make_unique<Atom>(get<Atom>(noad.value))
            ))));
        }
    }
    else if (superscript) {
        superscript = false;
        auto old_noad = std::move(result.back());
        result.pop_back();
        auto& atom = get<Atom>(old_noad.value);
        if (holds_alternative<AtomSuperscript>(atom.value)) {
            throw GeXError(token.group, token.string_index,
                    "Double superscript");
        }
        else if (holds_alternative<AtomSubsuperscript>(atom.value)) {
            throw GeXError(token.group, token.string_index,
                    "Double superscript");
        }
        else if (auto sub = get_if<AtomSubscript>(&atom.value)) {
            add_noad(Noad(Atom(Box(), sub->nucleus->type, AtomSubsuperscript(
                std::move(sub->nucleus),
                std::move(sub->subscript),
                std::make_unique<Atom>(get<Atom>(noad.value))
            ))));
        }
        else {
            add_noad(Noad(Atom(Box(), atom.type, AtomSuperscript(
                std::make_unique<Atom>(std::move(atom)),
                std::make_unique<Atom>(get<Atom>(noad.value))
            ))));
        }
    }
    else if (accent > 0) {
        auto& accent_noad = result.back();
        auto& accent_atom = get<AtomAccent>(get<Atom>(accent_noad.value).value);
        --accent;
        if (!accent_atom.accent) {
            accent_atom.accent = std::make_unique<Atom>(get<Atom>(noad.value));
        }
        else {
            accent_atom.nucleus = std::make_unique<Atom>(get<Atom>(noad.value));
        }
    }
    else {
        result.push_back(std::move(noad));
    }
}

double Processor::read_dimension()
{
    auto parser = DimensionParser();
    while (!parser.finished()) {
        try {
            if (token_index == ssize(tokens)) {
                parser.push(parser.end_token());
                break;
            }
        }
        catch (std::invalid_argument&) {
            throw GeXError(-1, -1, "Syntax error while reading a dimension");
        }
        auto& token = current_token();
        try {
            bool pushed = false;
            if (auto tok = get_if<CharacterToken>(&token.value)) {
                auto c = tok->codepoint;
                if (c >= '0' and c <= '7') {
                    pushed = parser.push(parser.OctalDigit_token(c));
                }
                else if (c >= '8' and c <= '9') {
                    pushed = parser.push(parser.DecimalDigit_token(c));
                }
                else if (c >= 'A' and c <= 'F') {
                    pushed = parser.push(parser.HexDigit_token(c));
                }
                else if (c == ' ') {
                    pushed = parser.push(parser.Space_token(' '));
                }
                else if (
                    c == '.' or c == ','  or c == 'p' or c == 't' or c == 'c' or
                    c == 'i' or c == 'n'  or c == 'b' or c == 'm' or c == 'd' or
                    c == 's' or c == '\'' or c == '"' or c == '+' or c == '-')
                {
                    pushed = parser.push(
                        parser.builtin_token(std::string(1, c)));
                }
                else {
                    pushed = parser.push(parser.end_token());
                }
            }
            else {
                pushed = parser.push(parser.end_token());
            }
            if (pushed) ++token_index;
        }
        catch (std::invalid_argument&) {
            throw GeXError(token.group, token.string_index,
                "Syntax error while parsing a dimension");
        }
    }
    return parser.get_result() / 10.0;
}


MathList gex::make_math_list(const TokenList& tokens)
{
    auto processor = Processor(tokens);
    processor.process();
    return processor.get_result();
}
