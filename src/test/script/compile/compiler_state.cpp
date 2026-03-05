#include <catch2/catch_test_macros.hpp>

#include "script/compile/compiler_state.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Labels", "[script]") {
    auto state = CompilerState();
    state.bytecode.resize(27);
    state.bytecode[0] = jump_long;
    state.bytecode[9] = jump_long;
    state.bytecode[18] = jump_long;
    state.labels.push_back({50, 0});
    state.labels.push_back({500, 1});
    state.labels.push_back({50000, 2});
    state.jumps.push_back({0, 0});
    state.jumps.push_back({9, 1});
    state.jumps.push_back({18, 2});
    state.resolve_labels();
    auto jump1 = 35;
    auto jump2 = 482;
    auto jump3 = 49987UL;
    auto bytes1 = reinterpret_cast<byte*>(&jump1);
    auto bytes2 = reinterpret_cast<byte*>(&jump2);
    auto bytes3 = reinterpret_cast<byte*>(&jump3);
    REQUIRE(state.bytecode.size() == 14);
    REQUIRE(state.bytecode[0] == jump_short);
    REQUIRE(state.bytecode[1] == bytes1[0]);
    REQUIRE(state.bytecode[2] == jump_medium);
    REQUIRE(state.bytecode[3] == bytes2[0]);
    REQUIRE(state.bytecode[4] == bytes2[1]);
    REQUIRE(state.bytecode[5] == jump_long);
    REQUIRE(state.bytecode[6] == bytes3[0]);
    REQUIRE(state.bytecode[7] == bytes3[1]);
    REQUIRE(state.bytecode[8] == bytes3[2]);
    REQUIRE(state.bytecode[9] == bytes3[3]);
    REQUIRE(state.bytecode[10] == bytes3[4]);
    REQUIRE(state.bytecode[11] == bytes3[5]);
    REQUIRE(state.bytecode[12] == bytes3[6]);
    REQUIRE(state.bytecode[13] == bytes3[7]);
}
