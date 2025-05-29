#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "ganim/object/text/gex/split.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    TokenList convert_to_tokens(std::string_view str)
    {
        auto result = TokenList();
        int i = 0;
        for (auto c : str) {
            auto cat = CategoryCode::Letter;
            if (c == '$') cat = CategoryCode::MathShift;
            result.emplace_back(CharacterToken(c, cat), 0, i++);
        }
        return result;
    }
}

TEST_CASE("GeX split", "[object][text][gex]") {
    auto tokens0 = convert_to_tokens("def");
    auto tokens1 = convert_to_tokens("abc");
    auto split1 = split(tokens1);
    REQUIRE(split1.size() == 1);
    REQUIRE(split1[0].tokens == tokens1);
    REQUIRE(split1[0].type == Section::Text);

    auto tokens2 = convert_to_tokens("$abc$");
    auto split2 = split(tokens2);
    REQUIRE(split2.size() == 1);
    REQUIRE(split2[0].tokens == tokens1);
    REQUIRE(split2[0].type == Section::InlineMath);

    auto tokens3 = convert_to_tokens("$$abc$$");
    auto split3 = split(tokens3);
    REQUIRE(split3.size() == 1);
    REQUIRE(split3[0].tokens == tokens1);
    REQUIRE(split3[0].type == Section::DisplayMath);

    auto tokens4 = convert_to_tokens("abc$def$");
    auto split4 = split(tokens4);
    REQUIRE(split4.size() == 2);
    REQUIRE(split4[0].tokens == tokens1);
    REQUIRE(split4[0].type == Section::Text);
    REQUIRE(split4[1].tokens == tokens0);
    REQUIRE(split4[1].type == Section::InlineMath);

    REQUIRE_THROWS_WITH(split(convert_to_tokens("$a")), "GeX compilation "
            "error in group 0 index 1: Expected math shift character");
    REQUIRE_THROWS_WITH(split(convert_to_tokens("$$a")), "GeX compilation "
            "error in group 0 index 2: Expected math shift character");
    REQUIRE_THROWS_WITH(split(convert_to_tokens("$$a$")), "GeX compilation "
            "error in group 0 index 3: Expected math shift character");
    REQUIRE_THROWS_WITH(split(convert_to_tokens("$$$a$$$")), "GeX compilation "
            "error in group 0 index 2: Three math shift characters in a row are"
            " not permitted.");
}
