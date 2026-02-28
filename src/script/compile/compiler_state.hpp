#ifndef GANIM_SCRIPT_COMPILER_STATE_HPP
#define GANIM_SCRIPT_COMPILER_STATE_HPP

#include <vector>
#include <cstdint>

namespace ganim {
    using std::byte;
    using std::int64_t;
    using std::uint64_t;
    struct CompilerState {
        std::vector<std::byte> bytecode;

        void write_parameter(std::byte value);
        void write_parameter(std::int64_t value);
        void write_parameter(std::uint64_t value);
        void write_parameter(double value);
    };
}

#endif
