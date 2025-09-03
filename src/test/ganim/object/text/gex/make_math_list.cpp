#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/make_math_list.hpp"

using namespace ganim::gex;

TEST_CASE("GeX make_math_list basic", "[object][text][gex]") {
    auto tokens = TokenList();

    tokens.emplace_back(CharacterToken(U'H', CategoryCode::Other), 0, 0);
    tokens.emplace_back(CharacterToken(U'=', CategoryCode::Other), 1, 0);
    tokens.emplace_back(CharacterToken(U'+', CategoryCode::Other), 2, 0);

    auto list = make_math_list(tokens);
    REQUIRE(list.size() == 3);
    auto atom = &get<Atom>(list[0].value);
    REQUIRE(atom->type == AtomType::Ord);
    auto& symbol1 = get<AtomSymbol>(atom->value);
    REQUIRE(symbol1.codepoint == U'H');
    REQUIRE(symbol1.group == 0);
    REQUIRE(symbol1.string_index == 0);

    atom = &get<Atom>(list[1].value);
    REQUIRE(atom->type == AtomType::Rel);
    auto& symbol2 = get<AtomSymbol>(atom->value);
    REQUIRE(symbol2.codepoint == U'=');
    REQUIRE(symbol2.group == 1);
    REQUIRE(symbol2.string_index == 0);

    atom = &get<Atom>(list[2].value);
    REQUIRE(atom->type == AtomType::Bin);
    auto& symbol3 = get<AtomSymbol>(atom->value);
    REQUIRE(symbol3.codepoint == U'+');
    REQUIRE(symbol3.group == 2);
    REQUIRE(symbol3.string_index == 0);
}

TEST_CASE("GeX make_math_list removing spaces", "[object][text][gex]") {
    auto tokens = TokenList();

    tokens.emplace_back(CharacterToken(U'H', CategoryCode::Other), 0,0);
    tokens.emplace_back(CharacterToken(U' ', CategoryCode::Other), 1,0);
    tokens.emplace_back(CharacterToken(U'e', CategoryCode::Other), 2,0);

    auto list = make_math_list(tokens);
    REQUIRE(list.size() == 2);
}

TEST_CASE("GeX make_math_list commands", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CommandToken(U"aaa", "aaa"), 0, 0);
    tokens.emplace_back(CommandToken(U"bbb", "bbb"), 1, 0);
    auto list = make_math_list(tokens);
    REQUIRE(list.size() == 2);
    auto& command1 = get<CommandNoad>(list[0].value);
    auto& command2 = get<CommandNoad>(list[1].value);
    REQUIRE(command1.command == "aaa");
    REQUIRE(command2.command == "bbb");
    REQUIRE(command1.group == 0);
    REQUIRE(command2.group == 1);
    REQUIRE(command1.string_index == 0);
    REQUIRE(command2.string_index == 0);
}

TEST_CASE("GeX make_math_list groups", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CharacterToken(U'a', CategoryCode::Other), 0, 0);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 1, 0);
    tokens.emplace_back(CharacterToken(U'b', CategoryCode::Other), 1, 0);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 1, 0);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 2);
    auto& atom1 = get<Atom>(list[0].value);
    auto& atom2 = get<Atom>(list[1].value);
    REQUIRE(atom1.type == AtomType::Ord);
    REQUIRE(atom2.type == AtomType::Ord);
    auto& symbol1 = get<AtomSymbol>(atom1.value);
    auto& sub_list = get<AtomList>(atom2.value).list;
    REQUIRE(symbol1.codepoint == U'a');
    REQUIRE(symbol1.group == 0);
    REQUIRE(sub_list.size() == 1);
    auto& atom3 = get<Atom>(sub_list[0].value);
    REQUIRE(atom3.type == AtomType::Ord);
    auto& symbol2 = get<AtomSymbol>(atom3.value);
    REQUIRE(symbol2.codepoint == U'b');
    REQUIRE(symbol2.group == 1);
}

TEST_CASE("GeX make_math_list subscripts/superscripts", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CharacterToken(U'a', CategoryCode::Other), 0, 0);
    tokens.emplace_back(CharacterToken(U'_', CategoryCode::Subscript), 1, 0);
    tokens.emplace_back(CharacterToken(U'b', CategoryCode::Other), 1, 0);
    tokens.emplace_back(CharacterToken(U'^', CategoryCode::Superscript), 2, 0);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 2, 0);
    tokens.emplace_back(CharacterToken(U'c', CategoryCode::Other), 2, 0);
    tokens.emplace_back(CharacterToken(U'd', CategoryCode::Other), 2, 0);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 2, 0);
    tokens.emplace_back(CharacterToken(U'e', CategoryCode::Other), 3, 0);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 2);
    auto& atom1 = get<Atom>(list[0].value);
    auto& atom2 = get<Atom>(list[1].value);
    REQUIRE(atom1.type == AtomType::Ord);
    REQUIRE(atom2.type == AtomType::Ord);
    auto& base = get<AtomSubsuperscript>(atom1.value);
    REQUIRE(base.nucleus->type == AtomType::Ord);
    REQUIRE(base.subscript->type == AtomType::Ord);
    REQUIRE(base.superscript->type == AtomType::Ord);
    auto& nucleus = get<AtomSymbol>(base.nucleus->value);
    auto& subscript = get<AtomSymbol>(base.subscript->value);
    auto& superscript = get<AtomList>(base.superscript->value).list;
    REQUIRE(nucleus.codepoint == U'a');
    REQUIRE(nucleus.group == 0);
    REQUIRE(subscript.codepoint == U'b');
    REQUIRE(subscript.group == 1);
    REQUIRE(superscript.size() == 2);
    auto& atom3 = get<Atom>(superscript[0].value);
    auto& atom4 = get<Atom>(superscript[1].value);
    REQUIRE(atom3.type == AtomType::Ord);
    REQUIRE(atom4.type == AtomType::Ord);
    auto& superscript1 = get<AtomSymbol>(atom3.value);
    auto& superscript2 = get<AtomSymbol>(atom4.value);
    REQUIRE(superscript1.codepoint == U'c');
    REQUIRE(superscript1.group == 2);
    REQUIRE(superscript2.codepoint == U'd');
    REQUIRE(superscript2.group == 2);
    auto& symbol = get<AtomSymbol>(atom2.value);
    REQUIRE(symbol.codepoint == U'e');
    REQUIRE(symbol.group == 3);
}

TEST_CASE("GeX make_math_list accents", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CommandToken(U"mathaccent", "mathaccent"), 0, 0);
    tokens.emplace_back(CharacterToken(U'a', CategoryCode::Other), 1, 0);
    tokens.emplace_back(CharacterToken(U'b', CategoryCode::Other), 2, 0);
    tokens.emplace_back(CommandToken(U"mathaccent", "mathaccent"), 3, 0);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 4, 0);
    tokens.emplace_back(CharacterToken(U'c', CategoryCode::Other), 5, 0);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 6, 0);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 7, 0);
    tokens.emplace_back(CharacterToken(U'd', CategoryCode::Other), 8, 0);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 9, 0);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 2);
    auto& atom1 = get<Atom>(list[0].value);
    auto& atom2 = get<Atom>(list[1].value);
    REQUIRE(atom1.type == AtomType::Acc);
    REQUIRE(atom2.type == AtomType::Acc);
    auto& accent_atom1 = get<AtomAccent>(atom1.value);
    auto& accent_atom2 = get<AtomAccent>(atom2.value);
    REQUIRE(accent_atom1.nucleus->type == AtomType::Ord);
    REQUIRE(accent_atom1.accent->type == AtomType::Ord);
    REQUIRE(accent_atom2.nucleus->type == AtomType::Ord);
    REQUIRE(accent_atom2.accent->type == AtomType::Ord);
    auto& nucleus1 = get<AtomSymbol>(accent_atom1.nucleus->value);
    auto& accent1 = get<AtomSymbol>(accent_atom1.accent->value);
    auto& nucleus2_list = get<AtomList>(accent_atom2.nucleus->value).list;
    auto& accent2_list = get<AtomList>(accent_atom2.accent->value).list;
    REQUIRE(nucleus2_list.size() == 1);
    REQUIRE(accent2_list.size() == 1);
    auto& nucleus2 = get<AtomSymbol>(get<Atom>(nucleus2_list[0].value).value);
    auto& accent2 = get<AtomSymbol>(get<Atom>(accent2_list[0].value).value);
    REQUIRE(accent1.codepoint == U'a');
    REQUIRE(accent1.group == 1);
    REQUIRE(nucleus1.codepoint == U'b');
    REQUIRE(nucleus1.group == 2);
    REQUIRE(accent2.codepoint == U'c');
    REQUIRE(accent2.group == 5);
    REQUIRE(nucleus2.codepoint == U'd');
    REQUIRE(nucleus2.group == 8);
}

TEST_CASE("GeX make_math_list fractions", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CharacterToken(U'a', CategoryCode::Other), 0, 0);
    tokens.emplace_back(CharacterToken(U'b', CategoryCode::Other), 0, 1);
    tokens.emplace_back(
        CommandToken(U"abovewithdelims", "abovewithdelims"), 0, 2);
    tokens.emplace_back(CharacterToken(U' ', CategoryCode::Spacer), 0, 3);
    tokens.emplace_back(CharacterToken(U'(', CategoryCode::Other), 0, 4);
    tokens.emplace_back(CharacterToken(U' ', CategoryCode::Spacer), 0, 5);
    tokens.emplace_back(CharacterToken(U'.', CategoryCode::Other), 0, 6);
    tokens.emplace_back(CharacterToken(U' ', CategoryCode::Spacer), 0, 7);
    tokens.emplace_back(CharacterToken(U'1', CategoryCode::Other), 0, 8);
    tokens.emplace_back(CharacterToken(U'p', CategoryCode::Other), 0, 9);
    tokens.emplace_back(CharacterToken(U't', CategoryCode::Other), 0, 10);
    tokens.emplace_back(CharacterToken(U'c', CategoryCode::Other), 0, 11);
    tokens.emplace_back(CharacterToken(U'd', CategoryCode::Other), 0, 12);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 1);
    auto& fraction = get<FractionNoad>(list[0].value);
    REQUIRE(fraction.left_delim == U'(');
    REQUIRE(fraction.right_delim == 0);
    REQUIRE(fraction.rule_thickness == 0.1);

    REQUIRE(fraction.numerator.size() == 2);
    auto& atom1 = get<Atom>(fraction.numerator[0].value);
    auto& atom2 = get<Atom>(fraction.numerator[1].value);
    auto& symbol1 = get<AtomSymbol>(atom1.value);
    auto& symbol2 = get<AtomSymbol>(atom2.value);
    REQUIRE(symbol1.codepoint == U'a');
    REQUIRE(symbol2.codepoint == U'b');

    REQUIRE(fraction.denominator.size() == 2);
    auto& atom3 = get<Atom>(fraction.denominator[0].value);
    auto& atom4 = get<Atom>(fraction.denominator[1].value);
    auto& symbol3 = get<AtomSymbol>(atom3.value);
    auto& symbol4 = get<AtomSymbol>(atom4.value);
    REQUIRE(symbol3.codepoint == U'c');
    REQUIRE(symbol4.codepoint == U'd');
}

TEST_CASE("GeX make_math_list mskip", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CharacterToken(U'a'), 0, 0);
    tokens.emplace_back(CommandToken(U"mskip", "mskip"), 0, 1);
    tokens.emplace_back(CharacterToken(U'3'), 0, 2);
    tokens.emplace_back(CharacterToken(U'm'), 0, 3);
    tokens.emplace_back(CharacterToken(U'u'), 0, 4);
    tokens.emplace_back(CharacterToken(U'b'), 0, 5);
    tokens.emplace_back(CommandToken(U"mskip", "mskip"), 0, 6);
    tokens.emplace_back(CharacterToken(U'-'), 0, 7);
    tokens.emplace_back(CharacterToken(U'3'), 0, 8);
    tokens.emplace_back(CharacterToken(U'm'), 0, 9);
    tokens.emplace_back(CharacterToken(U'u'), 0, 10);
    tokens.emplace_back(CharacterToken(U'c'), 0, 11);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 5);
    auto& symbol1 = get<AtomSymbol>(get<Atom>(list[0].value).value);
    auto& symbol2 = get<AtomSymbol>(get<Atom>(list[2].value).value);
    auto& symbol3 = get<AtomSymbol>(get<Atom>(list[4].value).value);
    REQUIRE(symbol1.codepoint == U'a');
    REQUIRE(symbol2.codepoint == U'b');
    REQUIRE(symbol3.codepoint == U'c');

    auto& glue1 = get<GlueNoad>(list[1].value);
    auto& glue2 = get<GlueNoad>(list[3].value);
    REQUIRE(glue1.thickness == 0.15);
    REQUIRE(glue2.thickness == -0.15);
}

TEST_CASE("make_math_list atom types", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CharacterToken(U'a'), 0, 0);
    tokens.emplace_back(CommandToken(U"mathbin", "mathbin"), 0, 1);
    tokens.emplace_back(CharacterToken(U'b'), 0, 2);
    tokens.emplace_back(CharacterToken(U'c'), 0, 3);
    tokens.emplace_back(CommandToken(U"mathrel", "mathbin"), 0, 4);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 0, 5);
    tokens.emplace_back(CharacterToken(U'd'), 0, 6);
    tokens.emplace_back(CharacterToken(U'e'), 0, 7);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 0, 8);
    tokens.emplace_back(CharacterToken(U'f'), 0, 9);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 5);
    auto& atom1 = get<Atom>(list[0].value);
    auto& atom2 = get<Atom>(list[1].value);
    auto& atom3 = get<Atom>(list[2].value);
    auto& atom4 = get<Atom>(list[3].value);
    auto& atom5 = get<Atom>(list[4].value);
    REQUIRE(atom1.type == AtomType::Ord);
    REQUIRE(atom2.type == AtomType::Bin);
    REQUIRE(atom3.type == AtomType::Ord);
    REQUIRE(atom4.type == AtomType::Bin);
    REQUIRE(atom5.type == AtomType::Ord);

    auto& group = get<AtomList>(atom4.value);
    REQUIRE(group.list.size() == 2);
    auto& atom4_1 = get<Atom>(group.list[0].value);
    auto& atom4_2 = get<Atom>(group.list[1].value);
    REQUIRE(atom4_1.type == AtomType::Ord);
    REQUIRE(atom4_2.type == AtomType::Ord);

    auto& symbol1 = get<AtomSymbol>(atom1.value);
    auto& symbol2 = get<AtomSymbol>(atom2.value);
    auto& symbol3 = get<AtomSymbol>(atom3.value);
    auto& symbol4 = get<AtomSymbol>(atom4_1.value);
    auto& symbol5 = get<AtomSymbol>(atom4_2.value);
    auto& symbol6 = get<AtomSymbol>(atom5.value);
    REQUIRE(symbol1.codepoint == U'a');
    REQUIRE(symbol2.codepoint == U'b');
    REQUIRE(symbol3.codepoint == U'c');
    REQUIRE(symbol4.codepoint == U'd');
    REQUIRE(symbol5.codepoint == U'e');
    REQUIRE(symbol6.codepoint == U'f');
}

TEST_CASE("GeX make_math_list radicals", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CommandToken(U"radical", "radical"), 0, 0);
    tokens.emplace_back(CharacterToken(U'a', CategoryCode::Other), 1, 0);
    tokens.emplace_back(CharacterToken(U'b', CategoryCode::Other), 2, 0);
    tokens.emplace_back(CommandToken(U"radical", "radical"), 3, 0);
    tokens.emplace_back(CharacterToken(U'c', CategoryCode::Other), 4, 0);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 5, 0);
    tokens.emplace_back(CharacterToken(U'd', CategoryCode::Other), 6, 0);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 7, 0);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 2);
    auto& atom1 = get<Atom>(list[0].value);
    auto& atom2 = get<Atom>(list[1].value);
    REQUIRE(atom1.type == AtomType::Rad);
    REQUIRE(atom2.type == AtomType::Rad);
    auto& radical_atom1 = get<AtomRadical>(atom1.value);
    auto& radical_atom2 = get<AtomRadical>(atom2.value);
    REQUIRE(radical_atom1.nucleus->type == AtomType::Ord);
    REQUIRE(radical_atom2.nucleus->type == AtomType::Ord);
    auto& nucleus1 = get<AtomSymbol>(radical_atom1.nucleus->value);
    auto& nucleus2_list = get<AtomList>(radical_atom2.nucleus->value).list;
    REQUIRE(nucleus2_list.size() == 1);
    auto& nucleus2 = get<AtomSymbol>(get<Atom>(nucleus2_list[0].value).value);
    REQUIRE(radical_atom1.radical == U'a');
    REQUIRE(radical_atom1.group == 1);
    REQUIRE(nucleus1.codepoint == U'b');
    REQUIRE(nucleus1.group == 2);
    REQUIRE(radical_atom2.radical == U'c');
    REQUIRE(radical_atom2.group == 4);
    REQUIRE(nucleus2.codepoint == U'd');
    REQUIRE(nucleus2.group == 6);
}
