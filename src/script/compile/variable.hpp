#ifndef GANIM_SCRIPT_COMPILE_VARIABLE_HPP
#define GANIM_SCRIPT_COMPILE_VARIABLE_HPP

#include "type.hpp"

namespace ganim {
    struct Variable {
        Type type;
        std::size_t address = -1;
        enum {StackFrame, Global} location;
        bool modifiable = true;
    };
}

#endif
