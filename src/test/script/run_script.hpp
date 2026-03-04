#ifndef GANIM_TEST_RUN_SCRIPT_HPP
#define GANIM_TEST_RUN_SCRIPT_HPP

#include <string>

#include "script/bytecode/interpreter.hpp"

namespace ganim {
    std::vector<Interpreter::TestType> run_script(
        std::string_view script,
        std::size_t final_stack_size = 0
    );
}

#endif
