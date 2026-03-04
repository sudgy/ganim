#include "run_script.hpp"

#include "script/parse/tokenize.hpp"
#include "script/parse/parse.hpp"
#include "script/compile/compile.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace ganim;

std::vector<Interpreter::TestType> ganim::run_script(
    std::string_view script,
    std::size_t final_stack_size
)
{
    auto tokens = tokenize(script);
    auto ast = parse(tokens);
    auto bytecode = compile(ast);
    auto interp = Interpreter(bytecode);
    interp.execute();
    INFO("All interpreter tests must leave a clean stack.");
    REQUIRE(interp.current_stack_size() == final_stack_size);
    return interp.get_test_output();
}
