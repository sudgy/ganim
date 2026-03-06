#include <catch2/catch_test_macros.hpp>

#include "script/compile/compiler.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Labels", "[script]") {
    auto compiler = Compiler({});
    compiler.write_jump(0);
    compiler.write_jump(1);
    compiler.write_jump(2);
    compiler.add_label_reference(50, 0);
    compiler.add_label_reference(500, 1);
    compiler.add_label_reference(50000, 2);
    compiler.resolve_labels();
    auto bytecode = compiler.take_bytecode();
    auto jump1 = 35;
    auto jump2 = 482;
    auto jump3 = 49987UL;
    auto bytes1 = reinterpret_cast<byte*>(&jump1);
    auto bytes2 = reinterpret_cast<byte*>(&jump2);
    auto bytes3 = reinterpret_cast<byte*>(&jump3);
    REQUIRE(bytecode.size() == 14);
    REQUIRE(bytecode[0] == jump_short);
    REQUIRE(bytecode[1] == bytes1[0]);
    REQUIRE(bytecode[2] == jump_medium);
    REQUIRE(bytecode[3] == bytes2[0]);
    REQUIRE(bytecode[4] == bytes2[1]);
    REQUIRE(bytecode[5] == jump_long);
    REQUIRE(bytecode[6] == bytes3[0]);
    REQUIRE(bytecode[7] == bytes3[1]);
    REQUIRE(bytecode[8] == bytes3[2]);
    REQUIRE(bytecode[9] == bytes3[3]);
    REQUIRE(bytecode[10] == bytes3[4]);
    REQUIRE(bytecode[11] == bytes3[5]);
    REQUIRE(bytecode[12] == bytes3[6]);
    REQUIRE(bytecode[13] == bytes3[7]);
}
