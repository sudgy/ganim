#ifndef GANIM_COMPILE_VALUE_HPP
#define GANIM_COMPILE_VALUE_HPP

#include "type.hpp"

namespace ganim {
    struct Value {
        Type type;
        enum {
            Global,
            StackFrame,
            RValue
        } location;
        uint64_t address = -1;
        bool modifiable = false;
    };
}

#endif
