#ifndef GANIM_SCRIPT_COMPILE_VARIABLE_HPP
#define GANIM_SCRIPT_COMPILE_VARIABLE_HPP

#include "script/type.hpp"

namespace ganim {
    struct Variable {
        Type type;
        std::size_t stack_frame_offset = -1;
        bool modifiable = true;
    };
}

#endif
