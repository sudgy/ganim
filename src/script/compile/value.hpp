#ifndef GANIM_COMPILE_VALUE_HPP
#define GANIM_COMPILE_VALUE_HPP

#include "type.hpp"

namespace ganim {
    struct Value {
        Type type;
        using stack_frame = std::uint64_t;
        using rvalue = std::monostate;
        std::variant<stack_frame, rvalue> location;
        bool modifiable = false;
    };
}

#endif
