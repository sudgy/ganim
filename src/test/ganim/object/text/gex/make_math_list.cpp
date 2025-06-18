#include <catch2/catch_test_macros.hpp>

#include "ganim/object/text/gex/make_math_list.hpp"

using namespace ganim::gex;

TEST_CASE("GeX make_atom_list basic", "[object][text][gex]") {
    auto section = Section();
    section.type = Section::InlineMath;

    section.tokens.emplace_back(CharacterToken(U'H', CategoryCode::Other), 0,0);
    section.tokens.emplace_back(CharacterToken(U'=', CategoryCode::Other), 1,0);
    section.tokens.emplace_back(CharacterToken(U'+', CategoryCode::Other), 2,0);

    auto list = make_math_list(section);
    REQUIRE(list.size() == 3);
    REQUIRE(list[0].group == 0);
    REQUIRE(list[0].string_index == 0);
    auto atom = &get<Atom>(list[0].value);
    REQUIRE(atom->type == AtomType::Ord);
    REQUIRE(get<AtomFieldSymbol>(atom->nucleus.value).codepoint == U'H');

    REQUIRE(list[1].group == 1);
    REQUIRE(list[1].string_index == 0);
    atom = &get<Atom>(list[1].value);
    REQUIRE(atom->type == AtomType::Rel);
    REQUIRE(get<AtomFieldSymbol>(atom->nucleus.value).codepoint == U'=');

    REQUIRE(list[2].group == 2);
    REQUIRE(list[2].string_index == 0);
    atom = &get<Atom>(list[2].value);
    REQUIRE(atom->type == AtomType::Bin);
    REQUIRE(get<AtomFieldSymbol>(atom->nucleus.value).codepoint == U'+');
}
