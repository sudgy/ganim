#include "run_script.hpp"

#include "script/parse/tokenize.hpp"
#include "script/parse/parse.hpp"
#include "script/compile/compile.hpp"

using namespace ganim;

std::vector<Interpreter::TestType> ganim::run_script(std::string_view script)
{
    auto tokens = tokenize(script);
    auto ast = parse(tokens);
    auto bytecode = compile(ast);
    auto interp = Interpreter(bytecode);
    interp.execute();
    return interp.get_test_output();
}
