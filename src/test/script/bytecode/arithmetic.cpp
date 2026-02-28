#include <catch2/catch_test_macros.hpp>

#include "script/bytecode/interpreter.hpp"
#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace bytecode;

TEST_CASE("Interpreter unary functions", "[script]") {
    auto code = std::vector<byte>{
        push_int,
        byte(5),
        unary_minus_int,
        test_int,
        pop,
        byte(1),

        push_double,
        param_byte8,
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        byte(0),
        unary_minus_double,
        test_double,
        pop,
        byte(1),
    };
    *reinterpret_cast<double*>(&code[8]) = 4.5;
    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 2);

    REQUIRE(get<int64_t>(output[0]) == -5);
    REQUIRE(get<double>(output[1]) == -4.5);
}

TEST_CASE("Interpreter basic binary arithmetic", "[script]") {
    auto code = std::vector<byte>{
        push_byte, byte(9), push_byte, byte(4), plus_byte,  test_byte,
        push_byte, byte(9), push_byte, byte(4), minus_byte, test_byte,
        push_byte, byte(9), push_byte, byte(4), mult_byte,  test_byte,
        push_byte, byte(9), push_byte, byte(4), div_byte,   test_byte,
        push_byte, byte(9), push_byte, byte(4), mod_byte,   test_byte,

        push_int, byte(9), push_int, byte(4), plus_int,  test_int,
        push_int, byte(9), push_int, byte(4), minus_int, test_int,
        push_int, byte(9), push_int, byte(4), mult_int,  test_int,
        push_int, byte(9), push_int, byte(4), div_int,   test_int,
        push_int, byte(9), push_int, byte(4), mod_int,   test_int,

        push_uint, byte(9), push_uint, byte(4), plus_uint,  test_uint,
        push_uint, byte(9), push_uint, byte(4), minus_uint, test_uint,
        push_uint, byte(9), push_uint, byte(4), mult_uint,  test_uint,
        push_uint, byte(9), push_uint, byte(4), div_uint,   test_uint,
        push_uint, byte(9), push_uint, byte(4), mod_uint,   test_uint,
    };

    auto d1 = 9.0;
    auto d2 = 4.0;
    auto d1_bytes = reinterpret_cast<byte*>(&d1);
    auto d2_bytes = reinterpret_cast<byte*>(&d2);
    for (auto b : {plus_double, minus_double, mult_double, div_double}) {
        code.push_back(push_double);
        code.push_back(param_byte8);
        for (int i = 0; i < 8; ++i) code.push_back(d1_bytes[i]);
        code.push_back(push_double);
        code.push_back(param_byte8);
        for (int i = 0; i < 8; ++i) code.push_back(d2_bytes[i]);
        code.push_back(b);
        code.push_back(test_double);
    }

    auto test = Interpreter(code);
    test.execute();
    auto& output = test.get_test_output();
    REQUIRE(output.size() == 19);

    REQUIRE(get<byte>(output[0]) == byte(13));
    REQUIRE(get<byte>(output[1]) == byte(5));
    REQUIRE(get<byte>(output[2]) == byte(36));
    REQUIRE(get<byte>(output[3]) == byte(2));
    REQUIRE(get<byte>(output[4]) == byte(1));

    REQUIRE(get<int64_t>(output[5]) == 13);
    REQUIRE(get<int64_t>(output[6]) == 5);
    REQUIRE(get<int64_t>(output[7]) == 36);
    REQUIRE(get<int64_t>(output[8]) == 2);
    REQUIRE(get<int64_t>(output[9]) == 1);

    REQUIRE(get<uint64_t>(output[10]) == 13);
    REQUIRE(get<uint64_t>(output[11]) == 5);
    REQUIRE(get<uint64_t>(output[12]) == 36);
    REQUIRE(get<uint64_t>(output[13]) == 2);
    REQUIRE(get<uint64_t>(output[14]) == 1);

    REQUIRE(get<double>(output[15]) == 13.0);
    REQUIRE(get<double>(output[16]) == 5.0);
    REQUIRE(get<double>(output[17]) == 36.0);
    REQUIRE(get<double>(output[18]) == 2.25);
}
