#ifndef GANIM_SCRIPT_COMPILER_STATE_HPP
#define GANIM_SCRIPT_COMPILER_STATE_HPP

#include "symbol_table.hpp"

namespace ganim {
    using std::byte;
    using std::int64_t;
    using std::uint64_t;
    struct CompilerState {
        std::vector<byte> bytecode;
        SymbolTable symbols;

        void write_parameter(byte value);
        void write_parameter(int64_t value);
        void write_parameter(uint64_t value);
        void write_parameter(double value);
    };
}

#endif
