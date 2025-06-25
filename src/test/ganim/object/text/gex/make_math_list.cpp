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
