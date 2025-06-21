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
    REQUIRE(list[0].group == 0);
    REQUIRE(list[1].group == 1);
    REQUIRE(list[0].string_index == 0);
    REQUIRE(list[1].string_index == 0);
    auto& command1 = get<CommandNoad>(list[0].value);
    auto& command2 = get<CommandNoad>(list[1].value);
    REQUIRE(command1.command == "aaa");
    REQUIRE(command2.command == "bbb");
}

TEST_CASE("GeX make_math_list groups", "[object][text][gex]") {
    auto tokens = TokenList();
    tokens.emplace_back(CharacterToken(U'a', CategoryCode::Other), 0, 0);
    tokens.emplace_back(CharacterToken(U'{', CategoryCode::StartGroup), 1, 0);
    tokens.emplace_back(CharacterToken(U'b', CategoryCode::Other), 1, 0);
    tokens.emplace_back(CharacterToken(U'}', CategoryCode::EndGroup), 1, 0);
    auto list = make_math_list(tokens);

    REQUIRE(list.size() == 2);
    REQUIRE(list[0].group == 0);
    REQUIRE(list[1].group == 1);
    auto& atom1 = get<Atom>(list[0].value);
    auto& atom2 = get<Atom>(list[1].value);
    REQUIRE(atom1.type == AtomType::Ord);
    REQUIRE(atom2.type == AtomType::Ord);
    auto& symbol1 = get<AtomFieldSymbol>(atom1.nucleus.value);
    auto& sub_list = get<AtomFieldList>(atom2.nucleus.value).list;
    REQUIRE(symbol1.codepoint == U'a');
    REQUIRE(sub_list.size() == 1);
    REQUIRE(sub_list[0].group == 1);
    auto& atom3 = get<Atom>(sub_list[0].value);
    REQUIRE(atom3.type == AtomType::Ord);
    auto& symbol2 = get<AtomFieldSymbol>(atom3.nucleus.value);
    REQUIRE(symbol2.codepoint == U'b');
}
